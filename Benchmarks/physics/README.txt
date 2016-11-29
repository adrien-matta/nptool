* This benchmark is used to check the performance of NPL::EnergyLoss and NPL::Reaction
  - The benchmark run 10000 cycles of Energy slow followed by EvaluateInitialEnergy and display the difference between the original energy and the final one obtained. It also provide the time to run the 10000 cycles
  -The benchmark run 10000 cycles of Generating a two body reaction at a given thetaCM followed by a reconstruction of the ThetaCM and diplay the difference between the original one and the final one obtained. It also provide the time to run the 10000 cycles

* To execute the benchmark: 
  $ root physics.cxx

* Check that the typical error in both case are less that 1 eV
