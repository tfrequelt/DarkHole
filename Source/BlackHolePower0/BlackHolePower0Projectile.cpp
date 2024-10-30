// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlackHolePower0Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ABlackHolePower0Projectile::ABlackHolePower0Projectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ABlackHolePower0Projectile::OnHit);		// set up a notification for when this component hits something blocking
	CollisionComp->SetGenerateOverlapEvents(true); // Activez les événements de chevauchement
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // Remplacez par le canal approprié
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Assurez-vous que les Pawns sont configurés pour chevaucher

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// Use a sphere as a simple collision representation
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CollisionComp);
	MeshComp->SetCollisionProfileName("NoCollision");
	MeshComp->SetGenerateOverlapEvents(true);
	MeshComp->SetVisibility(false);

	AttractionForce = 15000000.0f;

}


void ABlackHolePower0Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OnHit triggered!"));
	if (OtherActor && OtherActor != this && OtherComp && OtherComp->IsSimulatingPhysics() && !OtherActor->IsA(APawn::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Destroying Actor"));
		OtherActor->Destroy();
	}
}




void ABlackHolePower0Projectile::GenerateAttraction()
{		
	// debug  print
	UE_LOG(LogTemp, Warning, TEXT("GenerateAttraction"));
	// debug screen
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GenerateAttraction"));
	// Récupère tous les acteurs de type AActor dans un rayon défini (ex. 2000.0f)
	TArray<AActor*> OverlappingActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), OverlappingActors);
	// debug screen overlappingactors
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("OverlappingActors: %d"), OverlappingActors.Num()));

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor != this)
		{
			// Vérifie si c'est un objet physique
			UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
			if (PrimitiveComp && PrimitiveComp->IsSimulatingPhysics())
			{

				FVector Direction = GetActorLocation() - Actor->GetActorLocation();
				Direction.Normalize();
				// debug screen with direction
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Direction: %s"), *Direction.ToString()));
				PrimitiveComp->AddForce(Direction * AttractionForce); // Ajuste la force d’attraction selon les besoins
			}
		}
	}

}

void ABlackHolePower0Projectile::BeginPlay()
{
	Super::BeginPlay();
	BlackHoleLife = DistToExplode / ProjectileMovement->InitialSpeed;
	if (!GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("Timer active: %s"), GetWorld()->GetTimerManager().IsTimerActive(TimerHandle) ? TEXT("Yes") : TEXT("No"));
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABlackHolePower0Projectile::MyTimerFunction, BlackHoleLife, false);
	}

}



void ABlackHolePower0Projectile::MyTimerFunction()
{
	UE_LOG(LogTemp, Warning, TEXT("La fonction du timer a été appelée!"));
	// Ajoute ici le code que tu souhaites exécuter
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABlackHolePower0Projectile::GenerateAttraction, 0.1f, true);
	//debug screen
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("MyTimerFunction"));
	// arrete le mouvement de lacteur
	ProjectileMovement->StopMovementImmediately();
}