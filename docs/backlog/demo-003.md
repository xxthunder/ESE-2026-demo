# [DEMO-003] Architekturbilder zeigen Auto Off

**Status**: Open
**Priority**: Low
**Component**: `doc/software_architecture/_figures/`
**Related**: [DEMO-002](demo-002.md), Folgearbeit aus dem finalen Review

**Summary**:
Als Leser der Architekturdokumentation möchte ich Auto Off im Komponentendiagramm und im
Feature-Modell sehen, damit die Bilder den Code beschreiben, der gebaut wird.

**Description**:
DEMO-002 hat die Komponente `auto_off`, die RTE-Schnittstelle `RteSetAutoOffState()` /
`RteGetAutoOffState()` und das Feature `AUTO_OFF` mit `AUTO_OFF_TIMEOUT` eingeführt. Die Bilder
in `doc/software_architecture/_figures/` stammen aus `spled.drawio` und sind als SVG exportiert:
`sw_arch_component_diagram.svg` und `feature_model.svg`. Beide zeigen Auto Off nicht. DEMO-002
hat sie bewusst ausgelassen, weil die SVGs aus drawio exportiert werden.

**Acceptance Criteria**:
- [ ] `spled.drawio` zeigt die Komponente `auto_off`, ihre RTE-Schnittstellen und ihren Aufruf aus `spled`.
- [ ] Das Feature-Modell zeigt `AUTO_OFF` mit der Zeitdauer `AUTO_OFF_TIMEOUT`.
- [ ] `sw_arch_component_diagram.svg` und `feature_model.svg` sind neu aus `spled.drawio` exportiert, und der Bericht baut ohne neue Warnung.
