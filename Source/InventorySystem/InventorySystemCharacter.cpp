// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventorySystemCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "DrawDebugHelpers.h"
#include "Actors/Pickup.h"
#include "Actors/Tablet.h"
#include "Blueprint/UserWidget.h"
#include "Components/InventoryComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "UserInterface/InventorySystemHUD.h"
#include "UserInterface/Inventory/DragItemVisual.h"
#include "UserInterface/Inventory/InventoryTooltip.h"
DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AInventorySystemCharacter::AInventorySystemCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	TabletCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TabletCamera"));
	TabletCamera->SetupAttachment(GetMesh(), FName("head")); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	TabletCamera->bUsePawnControlRotation = false;

	WidgetInteractionComponent = CreateDefaultSubobject<UWidgetInteractionComponent>("Tablet Widget");	
	WidgetInteractionComponent->VirtualUserIndex = 1;
	WidgetInteractionComponent->InteractionSource = EWidgetInteractionSource::Mouse;
	WidgetInteractionComponent->bEnableHitTesting = true;
	WidgetInteractionComponent->bShowDebug = true;

	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Player Inventory"));
	PlayerInventory->SetSlotsCapacity(40);
	PlayerInventory->SetWeightCapacity(60);

	InteractionCheckFrequency = 0.1;
	InteractionCheckDistance = 225.f;

	// The point from which the line trace would start
	BaseEyeHeight = 74.0f;
}

void AInventorySystemCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AInventorySystemCharacter::Move);
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AInventorySystemCharacter::Look);
		// Interacting with items
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::BeginInteract);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ThisClass::EndInteract);
		// UWidgetInteractionComponent interactions
		EnhancedInputComponent->BindAction(MousePressed, ETriggerEvent::Started, this, &AInventorySystemCharacter::WidgetInteractPressed);
		EnhancedInputComponent->BindAction(MouseReleased, ETriggerEvent::Started, this, &AInventorySystemCharacter::WidgetInteractReleased);
		// Open Menu and inventory
		EnhancedInputComponent->BindAction(ToggleMenuAction, ETriggerEvent::Triggered, this, &ThisClass::ToggleMenu);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AInventorySystemCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AInventorySystemCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AInventorySystemCharacter::BeginPlay()
{	
	Super::BeginPlay();	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	HUDRef = Cast<AInventorySystemHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	//CreateUIWidgets();
}

void AInventorySystemCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime > InteractionCheckFrequency))
	{
		PerformInteractionCheck();
	}
}

void AInventorySystemCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	// to initialize a variable, same as TraceStart = FVector::Zero
	//FVector TraceStart{FVector::Zero()};

	FVector TraceStart{GetPawnViewLocation()};
	FVector TraceEnd{TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance)};

	// float LookDirection{FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector())};
	// the above wont work because the dot product return a double but our variable is float
	// and using the brackets forces to use the returned type only
	float LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());

	// as in dot products +ve means they are looking in same direction and -ve means opposite direction
	if(LookDirection > 0)
	{
		// Debug Linetrace Line
		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 2.0f);

		// for line trace collisions
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		// store the result of a line tarce
		FHitResult TraceHit;

		// using if because LineTrace returns true or false based on something we hit
		if(GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			// if the HitActor implements the Interface class
			// using the StaticClass return the C++ class
			if(TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{			
				// if we are looking and interacting with the same interactable
				// this will be false if we are looking different and interacting with a different Actor
				if(TraceHit.GetActor() != InteractionData.CurrentInteractable)
				{
					FoundInteractable(TraceHit.GetActor());
					return;
				}

				if(TraceHit.GetActor() == InteractionData.CurrentInteractable) return;
			}
		}		
	}
	// if none of the checks are true, that means we haven't found any interactable
	NoInteractableFound();
}

void AInventorySystemCharacter::FoundInteractable(AActor* NewInteractable)
{
	// to make sure that the previous interaction is not meddling with the new interactable
	if(IsInteracting())
	{
		EndInteract();
	}

	// there is a current Interactable
	if(InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	// Creates and Updates the HUD from the HUD class
	HUDRef->UpdateInteractionWidget(&TargetInteractable->InteractableData);

	// makes the new interactable as the focus
	TargetInteractable->BeginFocus();
}

void AInventorySystemCharacter::NoInteractableFound()
{
	// if we did not find any interactable clearing the timer manager
	if(IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandleInteraction);
	}

	if(InteractionData.CurrentInteractable)
	{
		if(IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}
		
		// hide interaction widget on HUD
		HUDRef->HideInteractionWidget();

		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}	
}

void AInventorySystemCharacter::BeginInteract()
{
	// to double check if we are interacting with the same thing since beginning inetraction
	PerformInteractionCheck();

	// timed interaction, like a progress bar to interact with an object
	if(IsValid(InteractionData.CurrentInteractable))
	{
		if(IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();

			if(FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				GetWorldTimerManager().SetTimer(TimerHandleInteraction,
					this,
					&AInventorySystemCharacter::Interact,
					TargetInteractable->InteractableData.InteractionDuration,
					false);
			}
		}
	}
}

void AInventorySystemCharacter::EndInteract()
{
	GetWorldTimerManager().ClearTimer(TimerHandleInteraction);

	if(IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

void AInventorySystemCharacter::Interact()
{
	GetWorldTimerManager().ClearTimer(TimerHandleInteraction);
	if(IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(this);
	}
}

void AInventorySystemCharacter::UpdateInteractionWidget() const
{
	if(IsValid(TargetInteractable.GetObject()))
	{
		HUDRef->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}

void AInventorySystemCharacter::ToggleMenu() 
{
	//HUDRef->ToggleMenu();
	if(bIsTabletVisible)
	{
		HideTablet();
	}
	else
	{
		ShowTablet();
	}
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	PlayerController->SetViewTargetWithBlend(this, 1.0f);
}

void AInventorySystemCharacter::ShowTablet()
{
	bIsTabletVisible = true;
	if(!CurrentTablet && TabletClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// tablet spawn transform and location
		FTransform SpawnTransform = GetMesh()->GetSocketTransform("TabletSocket");
		CurrentTablet = GetWorld()->SpawnActor<ATablet>(TabletClass, SpawnTransform, SpawnParams);
		CurrentTablet->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("TabletSocket"));		
	}
	if(CurrentTablet)
	{
		CurrentTablet->SetActorHiddenInGame(false);
		TabletCamera->SetActive(true);
		FollowCamera->SetActive(false);
		WidgetInteractionComponent->SetActive(true);

		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetHideCursorDuringCapture(false);
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = true;
		}

		if(UInventoryComponent* InventoryComponent = GetInventory())
		{
			InventoryComponent->OnInventoryUpdated.Broadcast();
		}
	}
	
}

void AInventorySystemCharacter::HideTablet()
{
	bIsTabletVisible = false;
	if(CurrentTablet)
	{
		CurrentTablet->SetActorHiddenInGame(true);
		TabletCamera->SetActive(false);
		FollowCamera->SetActive(true);
		WidgetInteractionComponent->SetActive(false);

		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = false;
		}
	}
}

void AInventorySystemCharacter::WidgetInteractPressed()
{
	WidgetInteractionComponent->PressPointerKey(EKeys::LeftMouseButton);	
}

void AInventorySystemCharacter::WidgetInteractReleased()
{
	WidgetInteractionComponent->ReleasePointerKey(EKeys::LeftMouseButton);	
}

// void AInventorySystemCharacter::CreateUIWidgets()
// {
// 	if(ToolTipWidgetClass)
// 	{
// 		ToolTipWidget = CreateWidget<UInventoryTooltip>(GetWorld(), ToolTipWidgetClass);
// 		ToolTipWidget->AddToViewport(100);
// 		ToolTipWidget->IsInViewport() ? GEngine->AddOnScreenDebugMessage(-98, 5.0f, FColor::Black, TEXT("ToolTipWidgetCreated")) : GEngine->AddOnScreenDebugMessage(-98, 5.0f, FColor::Black, TEXT("TollTipWidget not created"));
// 		ToolTipWidget->SetVisibility(ESlateVisibility::Hidden);
//
// 		// this is not working so either creae in a similar way like the inetraction widget in the HUD class or add another
// 		// UWidgetComponent inisd e the Tabelt class and hide and show it based on the interaction
// 		// either of the avove ways
// 	}
//
// 	if(DragVisualClass)
// 	{
// 		DragVisualWidget = CreateWidget<UDragItemVisual>(GetWorld(), DragVisualClass);
// 		DragVisualWidget->AddToViewport(100);
// 		DragVisualWidget->IsInViewport() ? GEngine->AddOnScreenDebugMessage(-98, 5.0f, FColor::Black, TEXT("DragWidget Created")) : GEngine->AddOnScreenDebugMessage(-98, 5.0f, FColor::Black, TEXT("DragWidget not created"));
// 		DragVisualWidget->SetVisibility(ESlateVisibility::Hidden);
// 	}
// }
//
// void AInventorySystemCharacter::UpdateToolTipWidget(const FVector2D& ScreenPosition, UItemBase* ItemIn)
// {
// 	if(ToolTipWidget)
// 	{
// 		UInventoryTooltip* Tooltip = Cast<UInventoryTooltip>(ToolTipWidget);
// 		if(Tooltip)
// 		{
// 			Tooltip->UpdateToolTipContents(ItemIn);
// 		}
// 		Tooltip->SetPositionInViewport(ScreenPosition, false);
// 		ToolTipWidget->SetVisibility(ESlateVisibility::Visible);
// 	}
// }
//
// void AInventorySystemCharacter::UpdateDragVisualWidget(const FVector2D& ScreenPosition, UItemBase* ItemIn)
// {
// 	if(DragVisualWidget)
// 	{
// 		UDragItemVisual* DragVisual = Cast<UDragItemVisual>(DragVisualWidget);
// 		if(DragVisual)
// 		{
// 			DragVisual->UpdateToolTipContents(ItemIn);
// 		}
// 		DragVisual->SetPositionInViewport(ScreenPosition, false);
// 		DragVisualWidget->SetVisibility(ESlateVisibility::Visible);
// 	}
// }
//
// void AInventorySystemCharacter::HideToolTipWidget()
// {
// 	if (ToolTipWidget)
// 	{
// 		ToolTipWidget->SetVisibility(ESlateVisibility::Hidden);
// 	}
// }
//
// void AInventorySystemCharacter::HideDragVisualWidget()
// {
// 	if (DragVisualWidget)
// 	{
// 		DragVisualWidget->SetVisibility(ESlateVisibility::Hidden);
// 	}
// }

void AInventorySystemCharacter::DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	if(PlayerInventory->FindMatchingItem(ItemToDrop))
	{
		// Spawning parameters for the actor
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		// Actor spawn location and rotation
		const FVector SpawnLocation{GetActorLocation()  + (GetActorForwardVector() * 50.0f)};
		const FTransform SpawnTransform{GetActorRotation(), SpawnLocation};

		const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

		APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), SpawnTransform, SpawnParams);

		// Updating the DataTable by calling the Drop Function from the Item itself 
		Pickup->InitializeDrop(ItemToDrop, RemovedQuantity);
	}
}


