/* 
 *            Copyright 2009-2012 The VOTCA Development Team
 *                       (http://www.votca.org)
 *
 *      Licensed under the Apache License, Version 2.0 (the "License")
 *
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef _VOTCA_CTP_COUPLINGH_H
#define _VOTCA_CTP_COUPLINGH_H

#include <stdio.h>

#include <votca/ctp/logger.h>
#include <votca/ctp/overlap.h>
#include <votca/ctp/qmpackagefactory.h>

namespace votca { namespace ctp {
    using namespace std;
    
class Coupling : public QMTool
{
public:

    Coupling() { };
   ~Coupling() { };

    string Identify() { return "coupling"; }

    void   Initialize(Property *options);
    bool   Evaluate();

 

private:
    
    string      _orbA, _orbB, _orbAB;
    string      _logA, _logB, _logAB;
    int         _levA, _levB;
    int         _trimA, _trimB;
    double      _degeneracy;

    string      _package;
    Property    _package_options; 
    
    string      _output_file;
    
    Logger      _log;

};

void Coupling::Initialize(Property* options) 
{

   // update options with the VOTCASHARE defaults   
    UpdateWithDefaults( options );
    std::string key = "options." + Identify();    
    
    _degeneracy = options->get(key + ".degeneracy").as<double> ();
    _package = options->get(key + ".package").as<string> ();
            
    _orbA  = options->get(key + ".moleculeA.orbitals").as<string> ();
    _orbB  = options->get(key + ".moleculeB.orbitals").as<string> ();
    _orbAB = options->get(key + ".dimerAB.orbitals").as<string> ();

    _logA  = options->get(key + ".moleculeA.log").as<string> ();
    _logB  = options->get(key + ".moleculeB.log").as<string> ();
    _logAB = options->get(key + ".dimerAB.log").as<string> ();

    _levA  = options->get(key + ".moleculeA.levels").as<int> ();
    _levB  = options->get(key + ".moleculeB.levels").as<int> ();
 
    _trimA  = options->get(key + ".moleculeA.trim").as<int> ();
    _trimB  = options->get(key + ".moleculeB.trim").as<int> ();
   
    _output_file = options->get(key + ".output").as<string> ();

    // get the path to the shared folders with xml files
    char *votca_share = getenv("VOTCASHARE");    
    if(votca_share == NULL) throw std::runtime_error("VOTCASHARE not set, cannot open help files.");
    string xmlFile = string(getenv("VOTCASHARE")) + string("/ctp/qmpackages/") + _package + string("_idft_pair.xml");
    load_property_from_xml( _package_options, xmlFile );    

    // register all QM packages (Gaussian, TURBOMOLE, etc)
    QMPackageFactory::RegisterAll();
    
}

bool Coupling::Evaluate() {

    _log.setReportLevel( logDEBUG );
    _log.setMultithreading( true );
    
    _log.setPreface(logINFO,    "\n... ...");
    _log.setPreface(logERROR,   "\n... ...");
    _log.setPreface(logWARNING, "\n... ...");
    _log.setPreface(logDEBUG,   "\n... ..."); 

    // get the corresponding object from the QMPackageFactory
    QMPackage *_qmpackage =  QMPackages().Create( _package );
   _qmpackage->setLog( &_log );       
   _qmpackage->Initialize( &_package_options );

     Orbitals _orbitalsA, _orbitalsB, _orbitalsAB;
  
    _qmpackage->setOrbitalsFileName( _orbA );
    bool _parse_orbitalsA_status = _qmpackage->ParseOrbitalsFile( &_orbitalsA );
    if ( !_parse_orbitalsA_status ) { LOG(logERROR,_log) << "Failed to read orbitals of molecule A" << std::flush; }

    _qmpackage->setOrbitalsFileName( _orbB );   
    bool _parse_orbitalsB_status = _qmpackage->ParseOrbitalsFile( &_orbitalsB );
    if ( !_parse_orbitalsB_status ) { LOG(logERROR,_log) << "Failed to read orbitals of molecule B" << std::flush; }
    
    _qmpackage->setOrbitalsFileName( _orbAB );   
    bool _parse_orbitalsAB_status = _qmpackage->ParseOrbitalsFile( &_orbitalsAB );
     if ( !_parse_orbitalsAB_status ) { LOG(logERROR,_log) << "Failed to read orbitals of dimer AB" << std::flush; }
   
    _qmpackage->setLogFileName( _logA );
    bool _parse_logA_status = _qmpackage->ParseLogFile( &_orbitalsA );
    if ( !_parse_logA_status ) { LOG(logERROR,_log) << "Failed to read log of molecule A" << std::flush; }
    
    _qmpackage->setLogFileName( _logB );
    bool _parse_logB_status = _qmpackage->ParseLogFile( &_orbitalsB );
    if ( !_parse_logB_status ) { LOG(logERROR,_log) << "Failed to read log of molecule B" << std::flush; }
    
    _qmpackage->setLogFileName( _logAB );
    bool _parse_logAB_status = _qmpackage->ParseLogFile( &_orbitalsAB );
    if ( !_parse_logAB_status ) { LOG(logERROR,_log) << "Failed to read log of molecule AB" << std::flush; }

    if ( _orbitalsA.getNumberOfElectrons()*(_trimA-1) <   _orbitalsA.getNumberOfLevels() - _orbitalsA.getNumberOfElectrons() ){

    
    LOG(logDEBUG,_log) << "Trimming virtual orbitals A:" 
                    << _orbitalsA.getNumberOfLevels() - _orbitalsA.getNumberOfElectrons() << "->" 
                    << _orbitalsA.getNumberOfElectrons()*(_trimA-1) << std::flush;  
    _orbitalsA.Trim(_trimA);
    }
    
    if ( _orbitalsA.getNumberOfElectrons()*(_trimA-1) <   _orbitalsA.getNumberOfLevels() - _orbitalsA.getNumberOfElectrons() ){

    LOG(logDEBUG,_log) << "Trimming virtual orbitals B:" 
                    << _orbitalsB.getNumberOfLevels() - _orbitalsB.getNumberOfElectrons() << "->" 
                    << _orbitalsB.getNumberOfElectrons()*(_trimB-1) << std::flush;      
    _orbitalsB.Trim(_trimB);
    }    
     Overlap _overlap; 
    _overlap.setLogger(&_log);
          
    ub::matrix<double> _JAB;
    bool _calculate_integrals = _overlap.CalculateIntegrals( &_orbitalsA, &_orbitalsB, &_orbitalsAB, &_JAB );  
    if ( !_calculate_integrals ) { LOG(logERROR,_log) << "Failed to evaluate integrals" << std::flush; }

     std::cout << _log;
 
     
     // output the results
    Property _summary; 
    Property *_job_output = &_summary.add("output","");
    Property *_pair_summary = &_job_output->add("pair","");
    int HOMO_A = _orbitalsA.getNumberOfElectrons();
    int HOMO_B = _orbitalsB.getNumberOfElectrons();
    int LUMO_A = HOMO_A + 1;
    int LUMO_B = HOMO_B + 1;
    _pair_summary->setAttribute("homoA", HOMO_A);
    _pair_summary->setAttribute("homoB", HOMO_B);
    for (int levelA = HOMO_A - _levA +1; levelA <= LUMO_A + _levA - 1; ++levelA ) {
        for (int levelB = HOMO_B - _levB + 1; levelB <= LUMO_B + _levB -1 ; ++levelB ) {        
                double JAB = _overlap.getCouplingElement( levelA , levelB, &_orbitalsA, &_orbitalsB, &_JAB, _degeneracy );
                Property *_overlap_summary = &_pair_summary->add("overlap", boost::lexical_cast<string>(JAB)); 
                double energyA = _orbitalsA.getEnergy( levelA );
                double energyB = _orbitalsB.getEnergy( levelB );
                _overlap_summary->setAttribute("orbA", levelA);
                _overlap_summary->setAttribute("orbB", levelB);
                //_overlap_summary->setAttribute("jAB", JAB);
                _overlap_summary->setAttribute("eA", energyA);
                _overlap_summary->setAttribute("eB", energyB);
        }
    }

    votca::tools::PropertyIOManipulator iomXML(votca::tools::PropertyIOManipulator::XML, 1, "");
     
    std::ofstream ofs (_output_file.c_str(), std::ofstream::out);
    ofs << *_job_output;    
    ofs.close();
    
    return true;
}


}}


#endif
