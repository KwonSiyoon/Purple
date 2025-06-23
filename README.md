
# 🎮 프로젝트 포트폴리오: Purple

## 📌 프로젝트 개요
- **프로젝트명**: Purple (가칭)
- **개발환경**: Unreal Engine 5.4, C++
- **장르**: 3D 액션 로그라이크 (Vampire Survivors 스타일)
- **개발 기간**: [직접 입력]
- **목표 플랫폼**: PC

---

## 🛠 담당 역할 및 구현 시스템

### 🔹 Skill 시스템
- **핵심 클래스**: `UPPSkillBase`, `UPPProjectileSkill`, `FPPProjectileData`, `APPProjectileBase`
- **구현 내용**:
  - 스킬 기본 클래스 구조 정의 및 하위 스킬 파생 구조 설계
  - Projectile 스킬은 DataTable에서 정보를 읽어 속성 결정
  - UI와 연동된 스킬 슬롯 시스템 구축
  - 자동 및 수동 발사 분기 처리

### 🔹 적 AI 시스템
- **핵심 클래스**: `BTTask_Approach`, `BTDecorator_AttackInRange`, `APPEnemyCharacterBase`, `PPAIController`
- **구현 내용**:
  - Behavior Tree 및 Blackboard 기반 AI 구성
  - 탐지 → 추적 → 공격 전환 구조 설계
  - EnemyBase 클래스에서 공통 이동/감지/공격 기능 구현

### 🔹 적 스폰 시스템
- **핵심 클래스**: `UPPWaveSpawnManager`, `FPPStageWave`, `FPPSpawnPattern`
- **구현 내용**:
  - 웨이브와 패턴 DataTable 기반 자동 스폰 구조 구축
  - 중첩 가능한 웨이브 구간 구현 (StartTime~EndTime)
  - 몬스터 위치는 OffscreenRandom 기반 난수로 최적화

### 🔹 UI 시스템
- **핵심 클래스**: `UPPHUDWidget`, `UPPSkillSlotWidget`
- **구현 내용**:
  - 스킬 UI 자동 생성 및 동적 바인딩 구현
  - 쿨타임 실시간 표시 및 스킬 아이콘 변경 반영
  - `APPPlayerController`에서 UI 처리 중심으로 재구성

### 🔹 경험치 및 레벨업 시스템
- **핵심 클래스**: `UPPDropComponent`, `AExpOrbActor`
- **구현 내용**:
  - 몬스터 처치 시 경험치 오브 드랍 및 자동 추적 흡수
  - 경험치 흡수 시 Player에 경험치 누적 및 HUD 반영

---

## 🧩 구조 설계 및 기술 스택

- 언리얼 공식 포인터 가이드 준수 (`TObjectPtr`, `TWeakObjectPtr` 사용)
- `ConstructorHelpers::FClassFinder`를 이용한 클래스 레퍼런싱
- DataTable 기반의 구조화된 스킬/웨이브/패턴/프로젝트일 데이터 관리
- Controller → Character → Skill → UI 흐름 분리로 유지보수 용이

---

## 🔧 기술적 문제 해결 사례

| 문제 상황 | 해결 방법 |
|-----------|------------|
| 웨이브 중첩 소환 시 Tick당 처리 부하 | FWaveRuntime 캐싱 및 Init단계 반복 설정 |
| HUD 생성 시점보다 UI 참조가 빠름 | PossessedBy 시점에 강제 바인딩 처리 |
| Projectile 방향 설정 불안정 | `GetActorForwardVector()` 기준 수직 정렬 보정 |

---

## 🧠 성장 포인트
- Unreal C++ 모듈 설계 감각 및 구조화 능력 강화
- 실시간 게임에서의 최적화 고려 (Tick 최소화, Init단계 분리)
- UI와 게임 로직의 바인딩 시점 문제 해결 역량 습득

---

## 📸 부가 자료
- **플레이 영상**: [YouTube 링크 또는 영상 주소 입력]
- **UML 다이어그램**: [링크 또는 첨부]
- **성능 프로파일링**: `Stat FPS`, `Stat Unit` 기반 캡처 포함

---

## 📂 전체 클래스 구조 예시 (간략)

```
APPPlayerController
  └── Possess() → AcquireSkill → HUD 바인딩 → Skill 슬롯 설정
  └── UseSkill → Skill Map 참조 → TryUseSkill()

UPPWaveSpawnManager
  └── Init() 시 StageWave, SpawnPattern 로딩
  └── Tick()에서 시간 조건 기반 스폰 처리

UPPSkillBase
  └── TryUseSkill() → UseSkill() 순 가상 함수 흐름
  └── 각 파생 클래스에서 발사체 방식, 범위 방식 등 분기
```
