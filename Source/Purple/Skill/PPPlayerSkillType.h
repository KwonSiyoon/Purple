#pragma once

#include "PPPlayerSkillType.generated.h"

UENUM(BlueprintType)
enum class EPlayerSkillType : uint8
{
    Empty      UMETA(DisplayName = "Empty"),
    Fireball   UMETA(DisplayName = "Fireball"),
    Iceball    UMETA(DisplayName = "Iceball"),
    Light      UMETA(DisplayName = "Light"),
    Heal       UMETA(DisplayName = "Heal"),
    Dash       UMETA(DisplayName = "Dash"),
    Max        UMETA(DisplayName = "Max")
};