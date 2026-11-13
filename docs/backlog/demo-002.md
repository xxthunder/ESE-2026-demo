# [DEMO-002] Auto Off

**Status**: In Progress
**Priority**: High
**Component**: `KConfig`, `components/`, `variants/`

**Summary**:
Als Nutzerin des Sleep Light möchte ich, dass das Licht nach einer Weile ohne Bedienung von
selbst ausgeht, damit es nicht die ganze Nacht brennt, wenn ich einschlafe.

**Description**:
Auto Off ist ein wählbares Feature der Produktlinie SPLed, kein fest eingebautes Verhalten
eines Produkts. Disco Light und Spa Light schalten sich nicht selbst aus und enthalten den Code
dafür nicht. Die Anforderungen stehen im Repository: REQ_39, REQ_40 und REQ_59.

**Acceptance Criteria**:
- [ ] Eine neue Komponente setzt die drei Anforderungen um.
- [ ] Das Feature ist über das Feature-Modell wählbar: ein Schalter und eine Zeitdauer.
- [ ] Sleep Light und IDEA/Sloemada wählen es mit 10 Sekunden, Disco Light und Spa Light nicht.
- [ ] Es gibt ein Detailed Design und Unit-Tests, die darauf verweisen.
- [ ] Alle Varianten bauen, alle Tests sind grün, und der Bericht zeigt Coverage und Traceability.

**Scope Decisions**:
- **Namen** (Entscheidung des Autors, 2026-11-13): Die Komponente heißt `components/auto_off`,
  ihr Kürzel ist `AO` (`SWDD_AO-<n>`, `SWIMPL_AO-<n>`, `TS_AO-<n>`). Der KConfig-Schalter heißt
  `AUTO_OFF`, wie in REQ_39 und REQ_59 als Konfiguration vermerkt. Die Zeitdauer heißt
  `AUTO_OFF_TIMEOUT`, in Sekunden, Bereich 5 bis 7200 wie in REQ_39.
- **Timer nur bei eingeschaltetem Licht** (Entscheidung des Autors, 2026-11-13): Der Timer läuft
  nur, solange der Power-State `ON` ist. Ist das Licht aus, gibt es nichts abzuschalten.
- **Bedienung** (Entscheidung des Autors, 2026-11-13): Jeder Takt, in dem Up oder Down gedrückt
  ist, setzt den Timer zurück; eine gehaltene Taste zählt also als andauernde Bedienung. Die
  Power-Taste zählt über ihr Event, einmal pro Druck.
- **Auto-Off-Zustand** (Entscheidung des Autors, 2026-11-13): `RteSetAutoOffState()` meldet einen
  Bool: `TRUE`, sobald der Timer das Licht ausgeschaltet hat, `FALSE` ab der nächsten Bedienung.
  Dazu kommt `RteGetAutoOffState()`. Keine andere Komponente liest ihn; die Konsole zeigt ihn
  nicht an.
