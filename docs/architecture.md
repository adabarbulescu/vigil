# vigil — Architecture

## Data Flow
```mermaid
graph TD
    A["/proc filesystem<br/>(Linux kernel interface)"] --> B["proc.c<br/>read_processes()"]
    A --> C["proc.c<br/>read_cpu_sample()"]
    
    B --> D["Sample A<br/>Process array + CPU ticks"]
    C --> E["CpuSample A<br/>System-wide ticks"]
    
    F["sleep(1 second)"] 

    B --> G["Sample B<br/>Process array + CPU ticks"]
    C --> H["CpuSample B<br/>System-wide ticks"]

    D --> I["proc.c<br/>calculate_cpu()<br/>PID-matched delta"]
    G --> I
    E --> I
    H --> I

    I --> J["display.c<br/>sort_by_cpu()"]
    J --> K["display.c<br/>display_processes()"]
    K --> L["Terminal output<br/>colored, sorted, filtered"]
```

## Per-process data sources

| Field | Source | Notes |
|-------|--------|-------|
| PID | `/proc/[pid]/` directory name | Numeric directories only |
| Name | `/proc/[pid]/comm` | Single line, stripped of newline |
| Memory | `/proc/[pid]/status` → `VmRSS` | Current resident set size in KB |
| CPU ticks | `/proc/[pid]/stat` | Fields 14 (utime) + 15 (stime) |

## System-wide CPU data

| Field | Source |
|-------|--------|
| Total ticks | `/proc/stat` first line — sum of all fields |
| Idle ticks | `/proc/stat` fields 4 + 5 (idle + iowait) |

## Two-sample CPU calculation
```
cpu% = (ΔprocessTicks / ΔsystemTicks) × 100

where:
  ΔprocessTicks = (utime_B + stime_B) - (utime_A + stime_A)
  ΔsystemTicks  = total_B - total_A
```
