//
//  AugmentedNetwork.cpp
//  CycleFormulation
//
//  Created by Carolina Riascos Alvarez.

#include "AugmentedNetwork.hpp"
#include "Class_Problem_VFS.hpp"

random_device rd2;
mt19937 gen(rd2());

vector<int>::iterator nextVeciCade(state& esta, bool sillego, int lv, int k, map<int,bool>& mOrden, double porcentaje, vector<int>& vVeci){
    vector<int>::iterator resul;
    if (lv >= k or esta.vamosVeci/double(esta.totalVeci) >= porcentaje){
        return esta.ItEnd;
    }
    if (sillego == false && lv >= k){
        return esta.ItEnd;
    }else{
        if (esta.firstTime == true){
            esta.firstTime = false;
            esta.vamosVeci++;
            resul = esta.Itestoy ;
        } else {
            esta.vamosVeci++;
            resul = esta.Itestoy + 1;
        }
    }
    return resul;
}
vector<int>::iterator nextVeci(state& esta, bool sillego, int lv, int k, map<int,bool>& mOrden){
    vector<int>::iterator resul;
    if (lv >= k){
        return esta.ItEnd;
    }
    if (sillego == false && lv >= k){
        return esta.ItEnd;
    }else{
        if (esta.firstTime == true){
            esta.firstTime = false;
            resul = esta.Itestoy ;
        } else {
            resul = esta.Itestoy + 1;
        }
    }
    return resul;
}
pair<bool,bool> siVaFin(vector<int>::iterator itVeci, int origen, map<int,vector<int>>& Comp2, map<int,pair<bool,bool>>& mapaFin, int voyPeque, int cuantosMDDsPeque, int lv){
    pair<bool, bool> pareja;
    bool sisi = true;
    if (voyPeque < cuantosMDDsPeque) sisi = lv <= 3 - 1;
    if (sisi == true){
        if (mapaFin.count(*itVeci) == 0){
            for (vector<int>::iterator it = Comp2[*itVeci].begin(); it != Comp2[*itVeci].end(); it++) {
                if (*it == origen) {
                    pareja.first = true;
                    if (lv == 3-1){
                        pareja.second = false;
                        return pareja;
                    }
                    if (Comp2[*itVeci].size() > 1) pareja.second = true;
                    mapaFin[*itVeci] = pareja;
                    return pareja;
                }
            }
            pareja.first = false;
            if (lv == 3-1){
                pareja.second = false;
                return pareja;
            }
            if (Comp2[*itVeci].size() > 0) pareja.second = true;
            mapaFin[*itVeci] = pareja;
            return pareja;
        } else {
            pair<bool,bool> aux = mapaFin[*itVeci];
            if (lv == 3-1){
                aux.second = false;
            }
            return aux;
        }
    }else{  // if del sisi
        pareja.first = false;
        pareja.second = false;
        return pareja;
    }
    
}
pair<bool,bool> siVaFinCade(vector<int>::iterator itVeci, map<int,vector<int>>& Comp2, map<int,pair<bool,bool>>& mapaFin, int lv, int ChainLength){
    pair<bool, bool> pareja;
    if (lv == ChainLength){
        pareja.first = true;
        pareja.second = false;
    } else if (Comp2[*itVeci].size() > 0){
        pareja.second = true;
        pareja.first = false;
    } else {
        pareja.first = false;
        pareja.second = false;
    }
    return pareja;
}
bool yaVisitado (state& estoy, int candidato){
    bool resp;
    if (estoy.vVisited.count(candidato) == 0){
        resp = false;
    }else{
        resp = true;
    }
    return resp;
}
bool suceIguales (state& estoy, state& otro, vector<state>& vLosNew){
    vector<int> vEstoy, vOtro;
    for (auto it : estoy.vSuce){
        if (vLosNew[it].id == -8){
            if (vLosNew[it].mActual[estoy].size() == vLosNew[it].mActual[otro].size()){
                for (auto it2 : vLosNew[it].mActual[estoy]){
                    vEstoy.push_back(it2);
                }
            }else{
                return false;
            }
        } else {
            vEstoy.push_back(vLosNew[it].actual);
        }
    }
    for (auto it : otro.vSuce){
        if (vLosNew[it].id == -8){
            for (auto it2 : vLosNew[it].mActual[otro]){
                vOtro.push_back(it2);
            }
        } else {
            vOtro.push_back(vLosNew[it].actual);
        }
    }
    sort(vEstoy.begin(), vEstoy.end());
    sort(vOtro.begin(), vOtro.end());
    
    bool resp = vEstoy == vOtro;
    
    return resp;
}
void mergeNodes (vector<state>& vLosNew, int lv, int estoy, Problem& P, vector<map<int,vector<int>>>& JustVisitedByState){
    //state jaja = estoy;
    for (int h = 0; h <  JustVisitedByState[lv-1][vLosNew[estoy].actual].size(); h++){ 
        if (vLosNew[JustVisitedByState[lv-1][vLosNew[estoy].actual][h]].id != vLosNew[estoy].id){
            if (vLosNew[estoy].vSuce.size() == vLosNew[JustVisitedByState[lv-1][vLosNew[estoy].actual][h]].vSuce.size()){
                bool suceIgual = suceIguales(vLosNew[estoy], vLosNew[JustVisitedByState[lv-1][vLosNew[estoy].actual][h]], vLosNew);
                if (suceIgual == true){ 
                    P.MergedNodes++;
                    for (auto j : vLosNew[estoy].vPrede){
                        for(auto m = vLosNew[j].vSuce.begin(); m != vLosNew[j].vSuce.end(); m++){
                            if (vLosNew[*m].id == vLosNew[estoy].id) {
                                *m = JustVisitedByState[lv-1][vLosNew[estoy].actual][h];
                            }
                        }
                        vLosNew[JustVisitedByState[lv-1][vLosNew[estoy].actual][h]].vPrede.push_back(j);  
                    }
                    for (auto j : vLosNew[estoy].vSuce){ 
                        if (j != 2){
                            for (auto z = 0; z < vLosNew[j].vPrede.size(); z++){ 
                                if (vLosNew[vLosNew[j].vPrede[z]].id == vLosNew[estoy].id){
                                    vLosNew[j].vPrede.erase(vLosNew[j].vPrede.begin() + z);
                                    break;
                                }
                            }
                        } else { 
                            auto itt = vLosNew[2].mActual.find(vLosNew[estoy]);
                            vLosNew[2].mActual.erase(itt);
                            for (auto z = 0; z < vLosNew[2].vPrede.size(); z++){
                                if (vLosNew[vLosNew[2].vPrede[z]].id == vLosNew[estoy].id){
                                    vLosNew[2].vPrede.erase(vLosNew[2].vPrede.begin() + z);
                                    break;
                                }
                            }
                        }
                    }
                    vLosNew[estoy].limpiar();
                    break;
                }
            }
        }
    }
}

//chains /////////////////////////////////////////
pair<vector<state>, vector<vector<int>>> Problem::BuildChainMDD(int iori, bool& hay, map<int,bool>& mOrden){
    int CuantosRegresosCero = 0;
    int origen = iori;
    map<int,vector<int>> Comp2 = Comp;

    
    vector<state> vLosNew;
    vector<vector<int>> vLvEstados;
    int lv = 1, id = 0;
    vLosNew.push_back(state(-1, -88, Comp2));
    vLosNew.push_back(state(id, origen, Comp2));
    int itNuevos = int(vLosNew.size()) - 1;
    vLosNew.back().vVisited[origen] = true;
    vLosNew.back().vPrede.push_back(0); 
    vLosNew[0].vSuce.push_back(1); 
    vLosNew.push_back(state(-8, -88, Comp2));
    int itDelFin = int(vLosNew.size()) - 1;
    
    vLvEstados.push_back(vector<int>());
    vLvEstados.back().push_back(itNuevos);
    map<int,pair<bool,bool>> mapaVaFin;
    map<int,bool> mapYaprede;
    int iLv = 0, jEs = 0; int jEsMer = 0;
    vector<vector<int>> vLvAfecta = vLvEstados; bool llegoMerge = false;
    
    double porceDeVeci = 0.18; // in percentage
    if (Nodes > 1500) porceDeVeci = 0.12;
    if (NDDs > 250) porceDeVeci = 0.08;
    int copyChainLength = 3;
    
    while (lv != 0){   //it creates the whole copy
        bool siLlego = false;
        auto it = nextVeciCade(vLosNew[vLvEstados[iLv][jEs]], siLlego, lv-1, int(copyChainLength), mOrden, porceDeVeci, Comp2[vLosNew[vLvEstados[iLv][jEs]].actual]);
        vLosNew[vLvEstados[iLv][jEs]].Itestoy = it;
        while(it != vLosNew[vLvEstados[iLv][jEs]].ItEnd){
            if (yaVisitado(vLosNew[vLvEstados[iLv][jEs]], *it) == false) {
                pair<bool, bool> vaOriMas = siVaFinCade(it, Comp2, mapaVaFin, lv, int(copyChainLength));
                bool directo = false;
                if (vaOriMas.first == true){ 
                    vLosNew[vLvEstados[iLv][jEs]].cadeRepe = true;
                    vLosNew[itDelFin].mActual[vLosNew[vLvEstados[iLv][jEs]]].push_back(*it);
                    if (vLosNew[vLvEstados[iLv][jEs]].meConectoDos == false){
                        vLosNew[vLvEstados[iLv][jEs]].meConectoDos = true;
                        vLosNew[vLvEstados[iLv][jEs]].vSuce.push_back(itDelFin);
                    }
                    if (mapYaprede.count(vLvEstados[iLv][jEs]) == 0) {
                        mapYaprede[vLvEstados[iLv][jEs]] = true;
                        vLosNew[itDelFin].vPrede.push_back(vLvEstados[iLv][jEs]);
                    }
                    directo = true; siLlego = true; llegoMerge = true;
                    if (hay == false) hay = true;
                }
                if (vaOriMas.second == true && lv < copyChainLength) {
                    lv++; id++;
                    vLosNew.push_back(state(id, *it, Comp2));
                    itNuevos = int(vLosNew.size()) - 1; 
                    vLosNew[itNuevos].vPrede.push_back(vLvEstados[iLv][jEs]);
                    vLosNew[itNuevos].vVisited =vLosNew[vLvEstados[iLv][jEs]].vVisited;
                    vLosNew[itNuevos].vVisited[vLosNew[itNuevos].actual] = true;
                    vLosNew[vLvEstados[iLv][jEs]].vSuce.push_back(itNuevos);
                    if (vLvEstados.size() < lv){
                        vLvEstados.push_back(vector<int>());
                        vLvAfecta.push_back(vector<int>());
                    }
                    iLv++;
                    if (copyChainLength >= 3 && iLv >= 2 && iLv <= copyChainLength - 1){
                       JustVisitedByState[iLv][vLosNew[itNuevos].actual].push_back(itNuevos);
                    }
                    vLvEstados[iLv].push_back(itNuevos);  // add iterator on this level
                    vLvAfecta[iLv].push_back(itNuevos);
                    jEs = int(vLvEstados[iLv].size()) - 1;
                    jEsMer = int(vLvAfecta[iLv].size() - 1);
                }
            } else { // si hay repetidos
                if (lv == int(copyChainLength) && it == vLosNew[vLvEstados[iLv][jEs]].ItEnd -1){
                    if (vLosNew[vLvEstados[iLv][jEs]].cadeRepe == false){
                        bool directo = false;
                        auto aaux = vLosNew[vLosNew[vLvEstados[iLv][jEs]].vPrede[0]];
                        vLosNew[itDelFin].mActual[aaux].push_back(vLosNew[vLvEstados[iLv][jEs]].actual);
                        if (vLosNew[vLvEstados[iLv][jEs]].meConectoDos == false){
                            vLosNew[vLvEstados[iLv][jEs]].meConectoDos = true;
                            vLosNew[vLvEstados[iLv][jEs]].vSuce.push_back(itDelFin);
                        }
                        if (mapYaprede.count(vLvEstados[iLv][jEs]) == 0) {
                            mapYaprede[vLvEstados[iLv][jEs]] = true;
                            vLosNew[vLvEstados[iLv][jEs]].meConectoDos = true;
                            vLosNew[itDelFin].vPrede.push_back(vLvEstados[iLv][jEs]);
                        }
                        directo = true; siLlego = false;
                        vLosNew[vLvEstados[iLv][jEs]].pathLlego = false;
                        if (hay == false) hay = true;
                        break;
                    }
                }
            }
            it = nextVeciCade(vLosNew[vLvEstados[iLv][jEs]], siLlego, lv-1, int(copyChainLength), mOrden, porceDeVeci, Comp2[vLosNew[vLvEstados[iLv][jEs]].actual]);
            vLosNew[vLvEstados[iLv][jEs]].Itestoy = it;
        }
        if (vLosNew[vLvEstados[iLv][jEs]].vSuce.size() == 0 && id != 0) {
            vLosNew[vLosNew[vLvEstados[iLv][jEs]].vPrede.back()].vSuce.pop_back();
            vLosNew.erase(vLosNew.begin() + vLvEstados[iLv][jEs]);
            vLvEstados[iLv].erase(vLvEstados[iLv].begin() + jEs);
            vLvAfecta[iLv].erase(vLvAfecta[iLv].begin() + jEsMer);
            id--;
        }
        lv--; 
        iLv--; 
        jEs = int(vLvEstados[iLv].size()) - 1;
        jEsMer = int(vLvAfecta[iLv].size() - 1);
        if (llegoMerge == true && iLv == 0 && jEs == 0) {  //merge
            llegoMerge = false;
            CuantosRegresosCero++;
            if(CuantosRegresosCero % CadaCuantoMerge == 0){
                for (auto ita = vLvAfecta.rbegin(); ita != vLvAfecta.rend(); ita++) {
                    int aulv = int(vLvAfecta.size() - 1 - (ita - vLvAfecta.rbegin())) + 1; // index de rit
                    if(aulv <= copyChainLength && aulv >= 3){
                       for (auto itb = ita->begin(); itb != ita->end();){
                            if (vLosNew[*itb].id != -88){
                                 mergeNodes(vLosNew, aulv, *itb, *this, this->JustVisitedByState);
                                itb = ita->erase(itb);
                            }
                       }
                    }
                }
            } // end modulo
        } // end if
    }
    vLvEstados.push_back(vector<int>(1,itDelFin));
    for (auto &i : vLosNew) i.eliminar88(vLosNew);
    return make_pair(vLosNew, vLvEstados);
}

//cycles /////////////////////////////////////////
pair<vector<state>, vector<vector<int>>> Problem::BuildCycleMDD(int iori, bool& hay, map<int,bool>& mOrden, int voyPeque, int cuantosMDDsPeque){
    int CuantosRegresosCero = 0;
    int origen = iori;
    map<int,vector<int>> Comp2 = Comp;
    if (mOrden.size() != 0){
        for (auto mi = 0; mi != Comp2.size(); mi++){
            auto mi2 = Comp2[mi].begin();
            for (; mi2 != Comp2[mi].end(); ) {
                if (mOrden.count(*mi2) == 1) {
                    mi2 = Comp2[mi].erase(mi2);
                } else {
                    mi2++;
                }
            }
        }
    }

    vector<state> vLosNew; 
    vector<vector<int>> vLvEstados; 
    int lv = 1, id = 0;
    vLosNew.push_back(state(-1, -88, Comp2));  // state preorigen
    vLosNew.push_back(state(id, origen, Comp2));  // state origen
    //vLosNew.back().sizeBytes();
    int itNuevos = int(vLosNew.size()) - 1;  //iterator of state origen
    vLosNew.back().vVisited[origen] = true;
    vLosNew.back().vPrede.push_back(0);  //prede preorigen
    vLosNew[0].vSuce.push_back(1);  // suce origen
    vLosNew.push_back(state(-8, -88, Comp2));  // state Fin
    int itDelFin = int(vLosNew.size()) - 1;  //iterator of state FIN (ahora int)
    vLosNew.push_back(state(-10, origen, Comp2));  // state Fin2
    int itDelFin2 = int(vLosNew.size()) - 1;
    vLosNew[int(vLosNew.size()) - 2].vSuce.push_back(int(vLosNew.size()) - 1);
    vLosNew[int(vLosNew.size()) - 1].vPrede.push_back(int(vLosNew.size()) - 2);
    
    vLvEstados.push_back(vector<int>());    // create levels
    vLvEstados.back().push_back(itNuevos);  // add iterator on this level
    map<int,pair<bool,bool>> mapaVaFin;
    map<int,bool> mapYaprede;
    int iLv = 0, jEs = 0; int jEsMer = 0;//through levels and states
    vector<vector<int>> vLvAfecta = vLvEstados; bool llegoMerge = false; //lv, a revisar los nuevos
    while (lv != 0){   //it creates the whole copy
        bool siLlego = false;
        auto it = nextVeci(vLosNew[vLvEstados[iLv][jEs]], siLlego, lv, int(CycleLength), mOrden);
        vLosNew[vLvEstados[iLv][jEs]].Itestoy = it;
        while(it != vLosNew[vLvEstados[iLv][jEs]].ItEnd){ // while there are neighbors
            if (yaVisitado(vLosNew[vLvEstados[iLv][jEs]], *it) == false) {
                pair<bool, bool> vaOriMas = siVaFin(it, origen, Comp2, mapaVaFin, voyPeque, cuantosMDDsPeque, lv);
                bool directo = false;
                if (vaOriMas.first == true){   // if veci goes until the end
                    TotalCycles++;
                    vLosNew[itDelFin].mActual[vLosNew[vLvEstados[iLv][jEs]]].push_back(*it);
                    auto ut = NonPresentNodes.find(*it);
                    if (ut != NonPresentNodes.end()) NonPresentNodes.erase(*it);
                    if (vLosNew[vLvEstados[iLv][jEs]].meConectoDos == false){
                        vLosNew[vLvEstados[iLv][jEs]].meConectoDos = true;
                        vLosNew[vLvEstados[iLv][jEs]].vSuce.push_back(itDelFin);
                    }
                    if (mapYaprede.count(vLvEstados[iLv][jEs]) == 0) {
                        mapYaprede[vLvEstados[iLv][jEs]] = true;
                        vLosNew[itDelFin].vPrede.push_back(vLvEstados[iLv][jEs]);
                    }
                    directo = true; siLlego = true; llegoMerge = true;
                    if (hay == false) hay = true;
                }
                if (vaOriMas.second == true && lv < CycleLength - 1) {
                    lv++; id++;
                    vLosNew.push_back(state(id, *it, Comp2));  // new node
                    itNuevos = int(vLosNew.size()) - 1;  //iterator of new node
                    vLosNew[itNuevos].vPrede.push_back(vLvEstados[iLv][jEs]);  //prede new state
                    vLosNew[itNuevos].vVisited =vLosNew[vLvEstados[iLv][jEs]].vVisited; // map copy of visited ones
                    vLosNew[itNuevos].vVisited[vLosNew[itNuevos].actual] = true; // visited new state
                    vLosNew[vLvEstados[iLv][jEs]].vSuce.push_back(itNuevos);    // successor of old state
                    if (vLvEstados.size() < lv){
                        vLvEstados.push_back(vector<int>()); // create lv
                        vLvAfecta.push_back(vector<int>());
                    }
                    iLv++;
                    if (CycleLength >= 4 && iLv >= 2 && iLv <= CycleLength - 2){
                       JustVisitedByState[iLv][vLosNew[itNuevos].actual].push_back(itNuevos);
                    }
                    vLvEstados[iLv].push_back(itNuevos);  // add iterator on this level
                    vLvAfecta[iLv].push_back(itNuevos);
                    jEs = int(vLvEstados[iLv].size()) - 1;
                    jEsMer = int(vLvAfecta[iLv].size()) - 1;
                }
            }
            it = nextVeci(vLosNew[vLvEstados[iLv][jEs]], siLlego, lv, int(CycleLength), mOrden);   //sig veci
            vLosNew[vLvEstados[iLv][jEs]].Itestoy = it;
            while(it != vLosNew[vLvEstados[iLv][jEs]].ItEnd && *it == origen){
                it = nextVeci(vLosNew[vLvEstados[iLv][jEs]], siLlego, lv, int(CycleLength), mOrden);   //sig veci
                vLosNew[vLvEstados[iLv][jEs]].Itestoy = it;
            }
        }
        if (vLosNew[vLvEstados[iLv][jEs]].vSuce.size() == 0 && id != 0) {
            vLosNew[vLosNew[vLvEstados[iLv][jEs]].vPrede.back()].vSuce.pop_back();
            vLosNew.erase(vLosNew.begin() + vLvEstados[iLv][jEs]);
            vLvEstados[iLv].erase(vLvEstados[iLv].begin() + jEs);
            vLvAfecta[iLv].erase(vLvAfecta[iLv].begin() + jEsMer);
            id--;
        }
        lv--;   //go down a level
        iLv--;  //reduce iterator
        jEs = int(vLvEstados[iLv].size()) - 1;
        jEsMer = int(vLvAfecta[iLv].size() - 1);
        if (llegoMerge == true && iLv == 0 && jEs == 0) {  //merge
            llegoMerge = false;
            CuantosRegresosCero++;
            if(CuantosRegresosCero % CadaCuantoMerge == 0){
                for (auto ita = vLvAfecta.rbegin(); ita != vLvAfecta.rend(); ita++) {
                    int aulv = int(vLvAfecta.size() - 1 - (ita - vLvAfecta.rbegin())) + 1; // index de ita
                    if(aulv <= CycleLength - 1 && aulv >= 3){
                        for (auto itb = ita->begin(); itb != ita->end();){
                            if (vLosNew[*itb].id != -88){
                                mergeNodes(vLosNew, aulv, *itb, *this, this->JustVisitedByState);
                                itb = ita->erase(itb);
                            }
                        }
                    }
                }
            } 
        } // end if
    }
    vLvEstados.push_back(vector<int>(1,itDelFin));
    vLvEstados.push_back(vector<int>(1,itDelFin2));
    for (auto &i : vLosNew) i.eliminar88(vLosNew);
    return make_pair(vLosNew, vLvEstados);
}

void Problem::BuildMDDs(){
    llenarComp();
    ordenFor = ordenDegre();//(DegreeType, P.Comp, P.CompPred, int(P.Nodes)); // first arc and value
    clock_t tStart = clock();
    tStart = clock();
    map<int,bool> mOrden; bool cade = false;
    pair<vector<state>, vector<vector<int>>> aux;  bool hay = false; // aux es una copia
    int conta = 0;
    int cuantosMDDsPeque = 0;
    int contaCade = 0;
    int cuantosMDDsCade = NDDs;
    if (Nodes > 500 && CycleLength >= 4) cuantosMDDsPeque = floor(Nodes*0.9);
    //if (Nodes > 2000 && CycleLength >= 4) cuantosMDDsPeque = floor(Nodes*0.90);
    for (auto i = 0; i < ordenFor.size(); i++){//
        hay = false;
        if (ordenFor[i].first > Pairs - 1 && ChainLength > 0){ // altruistic node
            if (cade == false){
                cade = true;
            }
            if (ChainLength >= 3) JustVisitedByState = vector<map<int,vector<int>>>(ChainLength);
            if (contaCade < cuantosMDDsCade){
                aux = BuildChainMDD(ordenFor[i].first, hay, mOrden);
                contaCade++;
                if(contaCade % 10 == 0) cout << "Chain number: " << contaCade << endl;
            }
        } else {  // cycle
            aux = BuildCycleMDD(ordenFor[i].first, hay, mOrden, conta, cuantosMDDsPeque);
        }
        if (hay == true){
                vMDD.push_back(aux);
                nstatesMDD+= aux.first.size();
                conta++;
        }
        if(i % 200 == 0) cout << "Number: " << i << endl;
        mOrden[ordenFor[i].first] = true;
    }
    MDDTime = (clock() - tStart)/double(CLOCKS_PER_SEC);
}
