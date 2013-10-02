# This file is here to help you making Analysis.
# Feel free to had more such as automatic lauch ou ROOT macro



cd src ; make ; cd ..
./Analysis ../Inputs/Reaction/34Si.reaction ../Inputs/DetectorConfiguration/e530.detector
root -l hit.c
