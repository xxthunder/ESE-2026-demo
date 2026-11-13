# [DEMO-004] Tastatur unter Linux einmal pro Takt lesen

**Status**: Open
**Priority**: Low
**Component**: `components/keyboard_interface/src/keyboard_interface.c`
**Related**: [DEMO-002](demo-002.md), Folgearbeit aus dem finalen Review

**Summary**:
Als Entwicklerin, die SPLed unter Linux startet, möchte ich, dass jede Komponente denselben
Tastendruck sieht, damit eine Taste nicht davon abhängt, welche Komponente zuerst fragt.

**Description**:
Unter Linux liest `KeyboardInterfaceIsKeyPressed()` bei jedem Aufruf ein Zeichen von stdin und
verwirft es. Jeder Aufruf von `RteIsKeyPressed()` nimmt also den anderen Lesern ein Zeichen weg.
Seit DEMO-002 fragt `autoOff()` pro Takt zusätzlich Up und Down ab, nach `powerButton()` und vor
`mainControlKnob()`. Unter Windows liest `GetAsyncKeyState()` den Tastenzustand ohne
Nebenwirkung; dort tritt das nicht auf.

**Acceptance Criteria**:
- [ ] Unter Linux wird stdin höchstens einmal pro Takt gelesen, und alle Aufrufe von `RteIsKeyPressed()` im selben Takt sehen dasselbe Ergebnis.
- [ ] Ein Test zeigt, dass zwei Abfragen derselben Taste im selben Takt beide `TRUE` liefern.
- [ ] Das Verhalten unter Windows bleibt unverändert.
