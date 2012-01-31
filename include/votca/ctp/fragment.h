/*
 * Copyright 2009-2011 The VOTCA Development Team (http://www.votca.org)
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

#ifndef __VOTCA_CTP_FRAGMENT_H
#define	__VOTCA_CTP_FRAGMENT_H

#include <votca/ctp/atom.h>
#include <fstream>



namespace votca { namespace ctp {

class Topology;
class Molecule;
class Segment;
    
/**
    \brief Rigid fragment. One conjugated segment contains several rigid fragments.

 * Apart from the position and orientation it has a pointer to a conjugated segment 
 * it belongs to as well as positions of atoms which belong to it of two types those 
 * which are generated by MD and those obtained by substitution of rigid copies
 */

class Fragment {
public:
     /// Constructor
     Fragment(int id, string name) : _id(id), _name(name), _symmetry(0) { }
    ~Fragment();
    
    void Rotate();    // rotates w.r.t. center of map
    void Translate();    

    inline void setTopology(Topology *container) { _top = container; }
    inline void setMolecule(Molecule *container) { _mol = container; }
    inline void setSegment(Segment *container)   { _seg = container; }
    void        AddAtom( Atom* atom );

    Topology   *getTopology() { return _top; }
    Molecule   *getMolecule() { return _mol; }
    Segment    *getSegment()  { return _seg; }    
    vector< Atom* > &Atoms() { return _atoms; }

    const int    &getId() const { return _id; }
    const string &getName() const { return _name; }

    void         setSymmetry(int symmetry) { _symmetry = symmetry; }
    int          getSymmetry() { return _symmetry; }

    void         calcPos();
    void         setPos(vec pos) { _CoM = pos; }
    const vec   &getPos() const { return _CoM; }

private:

    Topology    *_top;
    Molecule    *_mol;
    Segment     *_seg;

    vector < Atom* > _atoms;
    vector< double > _weights;

    string      _name;
    int         _id;

    int         _symmetry;
    vec         _CoM;    // Center of mapping


};

}}

#endif	/* __VOTCA_CTP_FRAGMENT_H */

