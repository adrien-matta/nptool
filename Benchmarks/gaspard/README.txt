To perform the gaspard benchmark:

1) Run the following simulation for 100000 events:

* Execute: npsimulation -D benchmark_gaspard.detector -E 132Sndp_benchmark.reaction -O benchmark_gaspard -B batch.mac
* In Geant4: /run/beamOn 1000000
* exit

2) Execute the ControlSimul.C macro in ROOT:

* root ControlSimul.C

3) Check that the generated plots are similar to the benchmark file reference.root


