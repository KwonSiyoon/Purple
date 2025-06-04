#pragma once

#include "PPPlayerSkillType.generated.h"

UENUM(BlueprintType)
enum class EPlayerSkillType : uint8
{
    None       UMETA(DisplayName = "None"),
    Fireball   UMETA(DisplayName = "Fireball"),
    Heal       UMETA(DisplayName = "Heal"),
    Dash       UMETA(DisplayName = "Dash")
};