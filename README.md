# A Branch-and-Price Algorithm Enhanced by Decision Diagrams for the Kidney Exchange Problem

## [paper](https://pubsonline.informs.org/doi/abs/10.1287/msom.2022.0192) / [code](https://github.com/CarolinaRiascos/BP-MDDs-KEP)

This repo contains the code used to generate the results in the paper. The algotihm supports the cycles-and-chains variant with L>= 3. 
The cycles-only variant is also supported.

# Abstract
***Problem definition:*** Kidney paired donation programs allow patients registered with an incompatible donor to receive a suitable kidney 
from another donor, as long as the latter's co-registered patient, if any, also receives a kidney from a different donor. The kidney exchange problem (KEP)
aims to find an optimal collection of kidney exchanges taking the form of cycles and chains. ***Methodology/results:***
We develop the first decomposition method that is able to consider long cycles and long chains for projected large realistic instances. 
Particularly, we propose a branch-and-price framework in which the pricing problems are solved (for the first time in packing problems in a digraph) through multi-valued decision diagrams.
We present a new upper bound on the optimal value of the KEP, obtained via our master problem.
Computational experiments show superior performance of our method over the state of the art by optimally solving almost all instances in the PrefLib library for multiple cycle and chain lengths.
***Managerial implications:*** Our algorithm also allows the prioritization of the solution composition, e.g., chains over cycles or vice versa, and we conclude, similar to previous findings,
that chains benefit the overall matching efficiency and highly sensitized patients.

# Setup

## Include CPLEX libraries in Makefile
The results were obtained with CPLEX_Studio1210. Please, update the Makefile included in this repo with your CPLEX version (if different from the one used here) and corresponding directory. 
Put the Makefile in the same folder of the cpp files. Change to the directory of the Makefile, and type in `make` in the Terminal. After that you should get an executable named "BP_MDD". In the Terminal type in `.\BP_MDD` to run the executable.

## Run Tests

`/data` contains the following instances:

- PrefLib, which we used for our analysis in Section 6.2. The instances are originally available in [this link](https://www.preflib.org/datasets#00036) under Kidney Data. 
Instances in this set are named as Kidney_N[PDPs+ NDDs]_A[NDDs]. Thus, an instance set Kidney_Matching_2355_307 means there are 2355 patient-donor pairs (PDPs) and non-directed donors (NDDs), 307 of which are NDDs
and the rest are patient-donor pairs. Instances within this instance set are named as KP_Instance#_N[PDPs+ NDDs]_A[NDDs].txt. Instance# can take values from 1 to 310, as there are 310 instances in total.

The txt files have the following structure: Nodes >> NDDs >> Pairs >> NumArcs >> AdjacencyList >> WeightMatrix. All values in WeightMatrix are unitary.

- HSP instances, which we used for our analysis in Section 6.3 are a subset of modified instances from the PrefLib library, as explained in that section. Instances in this set are named after
their original instances in the PrefLib library plus (i) the proportion of low-sensitized pairs over the total number of pairs (sigma) and (ii) the compatibility probability for low-sensitized pairs (pl).
Thus, an HSP instance KP_Num203_N537_A25_0.7_0.8_0.txt means that there are 537 PDPs+NDDs, 25 NDDs, sigma = 70%, and pl = 80%.
The 0 at the end is a default value for all HSP instances.

The txt files have the following structure: Nodes >> NDDs >> Pairs >> NumArcs >> AdjacencyList >> WeightMatrix >> PairsType. All values in WeightMatrix are unitary and every entry in PairsType takes a value 1 indicating a pair has a hihgly-sensitized patient
and 0 otherwise.

# Usage

BPMDD requires the following arguments:

1. Instance file path 
2. Output file path   
3. Maximum cycle length (K) 
4. Maximum chain length (L)
5. Degree type: It decides the order how vertices are selected to build cycle copies of the input graph. It can take each of the following strings:
	- Indegree
	- Outdegree
	- Totaldegree
	- Increasing (All vertices sorted in increasing order) 
	- Best_K-VFS: It solves a MIP trying to find the smallest feedback set (recommended only for small-size instances since such optimization problem is NP-Hard). 
6. Time limit in seconds
7. Preference: "CY" or "CH"
