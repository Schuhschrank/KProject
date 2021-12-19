// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPlayerRole : uint8
{
	NotParticipating,
	Traitor,
	Innocent,
	Detective,
};

/**
 *
 */
UENUM(BlueprintType)
enum class EEndMatchReason : uint8
{
	NoReason,
	EndedByHost,

	/* All values greater than or equal to this and less than TraitorsWon mean Innocents won.  */
	InnocentsWon,

	AllTraitorsDead,

	AllTasksCompleted,

	/* All values greater than or equal to this mean Traitors won. */
	TraitorsWon,

	AllInnocentsDead,
};
