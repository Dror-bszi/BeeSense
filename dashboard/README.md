# Dashboard

Where the data lands and where the beekeeper reads it.

Scaffold only: `server.py` prints the planned API and exits. The stack and host
are a Stage 3 decision, but the interface is already fixed by what the hub
sends and what the evaluation needs.

```bash
python3 dashboard/server.py   # prints the planned routes
```

Two things shape the design:

1. **The landing view is a ranked hive list**, not a graph wall. The stated use
   case is a beekeeper visiting a remote apiary every few weeks who needs to
   know which hive to open first.
2. **`POST /api/truth` is load-bearing.** Precision and recall cannot be
   computed without ground truth, and the PDR gets it from weekly inspections.
   If the dashboard does not make logging an inspection easy, the project has
   no way to prove its main target was met.

Build the ingest path first. The field trial needs data captured before it
needs anything drawn.
