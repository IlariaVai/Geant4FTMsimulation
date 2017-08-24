if [ ! -d  "myScan" ]; then  
	mkdir myScan
fi 
perl -i.bak -p -e 's/mythickness=500/mythickness=50/ig' /home/alice/FTMsimulation/TrGEMG4/trunk/src/GasDetectorConstruction.cc
make
./TrGEMG4 FTMsimulation.mac >& myScan/scanlog1.log
mv task.root myScan/Task1.root
perl -i.bak -p -e 's/mythickness=50/mythickness=100/ig' /home/alice/FTMsimulation/TrGEMG4/trunk/src/GasDetectorConstruction.cc
make
./TrGEMG4 FTMsimulation.mac >& myScan/scanlog2.log
mv task.root myScan/Task2.root
perl -i.bak -p -e 's/mythickness=100/mythickness=200/ig' /home/alice/FTMsimulation/TrGEMG4/trunk/src/GasDetectorConstruction.cc
make
./TrGEMG4 FTMsimulation.mac >& myScan/scanlog3.log
mv task.root myScan/Task3.root
perl -i.bak -p -e 's/mythickness=200/mythickness=500/ig' /home/alice/FTMsimulation/TrGEMG4/trunk/src/GasDetectorConstruction.cc
make
./TrGEMG4 FTMsimulation.mac >& myScan/scanlog4.log
mv task.root myScan/Task4.root


