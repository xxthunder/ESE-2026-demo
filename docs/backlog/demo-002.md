# [DEMO-002] Auto Off

**Status**: Open
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
