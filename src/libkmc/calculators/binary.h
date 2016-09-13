/*
 * Copyright 2009-2013 The VOTCA Development Team (http://www.votca.org)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef __VOTCA_KMC_BINARY_H
#define	__VOTCA_KMC_BINARY_H

#include <votca/kmc/state.h>
#include <votca/kmc/event.h>
#include <votca/kmc/graph.h>
#include <votca/kmc/carrier.h>
#include <votca/kmc/bnode.h>
#include <votca/kmc/carrierfactory.h>
#include <votca/kmc/eventfactory.h>
//#include <votca/kmc/property.h>

using namespace std;

namespace votca { namespace kmc {
   
class Binary : public KMCCalculator 
{
public:
    
    Binary() {};
   ~Binary() {};

    using KMCCalculator::Initialize;
    void Initialize(Property *options);
    bool EvaluateFrame();

protected:
   void RunKMC(void); 
            
private:
   
};


void Binary::Initialize(Property *options) {
    
    std::cout << "Hello from KMC binary" << endl;
    
}

bool Binary::EvaluateFrame() {
        
    RunKMC();
    return true;
}

void Binary::RunKMC() {

    std::cout << "Running KMC binary" << endl;

    Graph graph;
    std::string filename( "state.sql" );
    graph.Load( filename );
    //graph.Print();

    // register all carrier types
    CarrierFactory::RegisterAll();
    
    //Create a new electron
    Carrier* electron =  Carriers().Create("electron");
    //std::cout << "Carrier says I am of the type " << carrier->Type() << endl;
    
    //Create a new hole
    //Carrier* hole = Carriers().Create("hole");
    
    State state;
    state.AddCarrier( "electron" );
    //state.AddCarrier("hole");
    
    state.Save( "state.bin" );
    
    //state.Clear();
    
    state.Load( "state.bin" );
    
    // register all event types
    EventFactory::RegisterAll();
    
    //New event - hole transfer
    //Event* ht =  Events().Create( "hole transfer" );
    //std::cout << "Event which will occur:  " << ht->Type() << endl;;
    //ht->OnExecute( &state );
 
    //New event - electron transfer
    Event* et =  Events().Create( "electron transfer" );
    std::cout << "Event which will occur:  " << et->Type() << endl;
    
    /*
    et->AddElectron( electron );
    et->AddOrigin( electron->Node() );
    //in the loop over the neighbours
    et->AddDestination( electron->Node->Neighbour() )
    et->SetRate( rate );
    */
    
    et->OnExecute( &state );
    
}

}}


#endif	/* __VOTCA_KMC_BINARY_H */
