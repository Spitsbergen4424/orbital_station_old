# Необходимые изменения существующих классов ЛР №1

Новые классы этапа 2 используют два небольших расширения существующей модели. Они нужны потому, что в исходной версии `Module` доступны только константные геттеры энергопотребления, а `ResearchModule` пока не хранит признак временной блокировки исследований.

## 1. `include/modules/Module.h`

В публичную секцию класса `Module` добавьте:

```cpp
void applyEnergyConsumptionModifier(double modifier);
void resetEnergyConsumption() noexcept;
```

## 2. `src/modules/Module.cpp`

Добавьте реализацию:

```cpp
void Module::applyEnergyConsumptionModifier(double modifier) {
    if (modifier < 0.0) return;
    const int modified = static_cast<int>(baseEnergyConsumption_ * modifier + 0.5);
    energyConsumption_.setValue(modified);
}

void Module::resetEnergyConsumption() noexcept {
    energyConsumption_.setValue(baseEnergyConsumption_);
}
```

Метод использует уже существующую проверку `EnergyConsumption::setValue`. Модификатор рассчитывается от базового потребления, а не от текущего: повторный вызов не приводит к экспоненциальному росту расхода.

## 3. `include/modules/ResearchModule.h`

Добавьте в публичную секцию:

```cpp
void setResearchBlocked(bool blocked) noexcept;
bool isResearchBlocked() const noexcept;
```

Также добавьте в private-секцию `Module` поле, которое хранит исходное потребление:

```cpp
int baseEnergyConsumption_;
```

Для поля `baseEnergyConsumption_` в классе `Module` добавьте инициализацию в конструкторе `Module`: `baseEnergyConsumption_(energyConsumption)`. Поле должно быть объявлено в private-секции `Module`, рядом с `energyConsumption_`. Это гарантирует, что повторное применение глубокого космоса не будет умножать уже увеличенное значение повторно.

В private-секцию `ResearchModule` добавьте:

```cpp
bool researchBlocked_ = false;
```

## 4. `src/modules/ResearchModule.cpp`

Сохраните существующий конструктор и добавьте реализации:

```cpp
void ResearchModule::setResearchBlocked(bool blocked) noexcept {
    researchBlocked_ = blocked;
}

bool ResearchModule::isResearchBlocked() const noexcept {
    return researchBlocked_;
}
```

Если конструктор в вашем файле пока только делегирует параметры базовому классу, его можно оставить без изменений: `researchBlocked_` уже инициализируется значением `false`.

## Почему изменения сделаны именно так

- `Module` остаётся единой точкой изменения энергопотребления, стратегии не получают доступ к private-полям.
- Блокировка исследований хранится в `ResearchModule`, а не определяется по строке `getType()`.
- Флаг блокировки не подменяет состояние `ModuleState`: блокировка исследований и авария оборудования — разные понятия.
- При применении `EarthOrbit` или `DeepSpace` временная блокировка снимается. Поэтому эффект астероидного поля не остаётся навсегда после смены окружения.
