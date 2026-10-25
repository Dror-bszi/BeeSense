#!/usr/bin/env python3
"""BeeSense dashboard -- scaffold.

What the beekeeper actually needs (PDR 1.1, Table I) is not a wall of graphs:
it is "which hive do I open first, and why". So the landing view is a ranked
list of hives, and the per-hive detail is one click down.

Deployment is deliberately undecided here. The hub uploads over LTE to a cloud
endpoint; whether that endpoint is this process on a small VPS or a managed
service is a Stage 3 decision. What is fixed is the interface:

    POST /api/reports   <- hub, batched bs_report_t records drained from the spool
    POST /api/alerts    <- hub, detection events, sent out of band
    GET  /api/hives     -> ranked hive list for the landing view
    GET  /api/hives/<id>-> weight, band energies, temperatures, z-scores, alerts
    POST /api/truth     <- beekeeper's inspection notes (queen cells, swarms seen)

That last one is not an afterthought. Precision and recall cannot be computed
without ground truth, and the PDR gets it from weekly inspections -- so the
dashboard has to be the thing that collects it.
"""


ROUTES = [
    ("POST", "/api/reports",     "ingest a batch from the hub -> timeseries store"),
    ("POST", "/api/alerts",      "ingest a detection event -> notify the beekeeper's phone"),
    ("GET",  "/api/hives",       "hives ranked by urgency: open this one first"),
    ("GET",  "/api/hives/<id>",  "per-hive history: weight, bands, temps, z, alerts"),
    ("POST", "/api/truth",       "weekly inspection notes -- the ground truth for P/R"),
    ("GET",  "/api/metrics",     "precision, recall, lead time, delivery ratio, battery"),
]


def main() -> None:
    print("BeeSense dashboard -- scaffold, no server running yet")
    print("\n  planned API:")
    for method, path, purpose in ROUTES:
        print(f"    {method:<5} {path:<20} {purpose}")
    print("\n  storage: per-hive timeseries (report every 15 min + event wakes)")
    print("           plus an inspection/ground-truth table and an alert log")
    print("\n  views:   1. ranked hive list (landing)")
    print("           2. per-hive: weight trend, band-energy heatmap over time, temps, alerts")
    print("           3. evaluation: precision/recall and lead-time distribution")
    print("\n  TODO: pick the stack and the host, then implement ingest first --")
    print("        the field trial needs data captured before it needs charts")


if __name__ == "__main__":
    main()
