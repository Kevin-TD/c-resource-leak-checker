#include "Domain.h"

/*
       Top
        | 
    MayNeedCall
    /      \
NeedsCall  DoesCall 
    \       / 
      Bottom

A may need call may or may not have appropriately, without error, allocated and deallocated some memory. Transfer functions for this type are a little tricky 
Top could be any random variable. Included for instances where we first declare a str (top) then call an allocation function (NeedsCall)

*/

// sep domains 
// 1 - allo domain 
// 2 - deallo/free domains { work on free domain - called methods https://checkerframework.org/manual/#introduction }
// 3 - compare 
// called methods use lattice on pg 5 https://dl.acm.org/doi/pdf/10.1145/3377811.3380341 



namespace dataflow {

Domain::Domain() { Value = Bottom; }
Domain::Domain(Element V) { Value = V; }

Domain* Domain::applyAllocate(Domain* D) {
    switch (D->Value) {
        case Top:
            return new Domain(NeedsCall);
            break;
        case MayNeedCall:
            return new Domain(MayNeedCall); 
            break;
        case NeedsCall:
            return new Domain(Bottom);
            break;
        case DoesCall:
            return new Domain(NeedsCall);
            break;
        case Bottom: 
            return new Domain(Bottom);
            break; 
    }
}

Domain* Domain::applyDeallocate(Domain* D) {
    switch (D->Value) {
        case Top:
            return new Domain(Bottom);
            break;
        case MayNeedCall:
            return new Domain(DoesCall); 
            break;
        case NeedsCall:
            return new Domain(DoesCall); 
            break;
        case DoesCall:
            return new Domain(Bottom);
            break;
        case Bottom: 
            return new Domain(Bottom);
            break; 
    }
}

Domain* Domain::applyReallocate(Domain* D) {
    switch (D->Value) {
        case Top:
            return new Domain(NeedsCall);
            break;
        case MayNeedCall:
            return new Domain(NeedsCall); 
            break;
        case NeedsCall:
            return new Domain(NeedsCall); 
            break;
        case DoesCall:
            return new Domain(NeedsCall);
            break;
        case Bottom: 
            return new Domain(Bottom);
            break; 
    }
}

Domain* Domain::applySafe(Domain* D) {
    return new Domain(D->Value);
}

Domain* Domain::applyUnsafe(Domain* D) {
    switch (D->Value) {
        case Top:
            return new Domain(Top);
            break;
        case MayNeedCall:
            return new Domain(MayNeedCall); 
            break;
        case NeedsCall:
            return new Domain(MayNeedCall); 
            break;
        case DoesCall:
            return new Domain(MayNeedCall);
            break;
        case Bottom: 
            return new Domain(Bottom);
            break; 
    }
}

void Domain::selfApplyAllocate() {
    switch (Value) {
        case Top:
            Value = NeedsCall;
            break;
        case MayNeedCall:
            Value = MayNeedCall; 
            break;
        case NeedsCall:
            Value = Bottom;
            break;
        case DoesCall:
            Value = NeedsCall;
            break;
        case Bottom: 
            Value = Bottom;
            break; 
    }
}

void Domain::selfApplyDeallocate() {
    switch (Value) {
        case Top:
            Value = Bottom;
            break;
        case MayNeedCall:
            Value = DoesCall; 
            break;
        case NeedsCall:
            Value = DoesCall; 
            break;
        case DoesCall:
            Value = Bottom;
            break;
        case Bottom: 
            Value = Bottom;
            break; 
    }
}

void Domain::selfApplyReallocate() {
    switch (Value) {
        case Top:
            Value = NeedsCall;
            break;
        case MayNeedCall:
            Value = NeedsCall; 
            break;
        case NeedsCall:
            Value = NeedsCall; 
            break;
        case DoesCall:
            Value = NeedsCall;
            break;
        case Bottom: 
            Value = Bottom;
            break; 
    }
}


void Domain::selfApplyUnsafe() {
    switch (Value) {
        case Top:
            Value = Top;
            break;
        case MayNeedCall:
            Value = MayNeedCall; 
            break;
        case NeedsCall:
            Value = MayNeedCall; 
            break;
        case DoesCall:
            Value = MayNeedCall;
            break;
        case Bottom: 
            Value = Bottom;
            break; 
    }
}

Domain Domain::join(Domain::Element E1, Domain::Element E2) {
    switch (E1) {
        case Top:
            return Domain(Top);
            break; 

        case MayNeedCall:
            switch (E2) {
                case Top:
                    return Domain(Top);
                    break;
                default: // MayNeedCall, NeedsCall, DoesCall, Bottom  
                    return Domain(MayNeedCall);
                    break; 
            }
            break; 

        case NeedsCall:
            switch (E2) {
                case Top:
                    return Domain(Top);
                    break;
                    
                case MayNeedCall:
                case DoesCall: 
                    return Domain(MayNeedCall); 
                    break;

                case NeedsCall:
                case Bottom: 
                    return Domain(NeedsCall); 
                    break;
            }
            break; 


            
        case DoesCall:
            switch (E2) {
                 case Top:
                    return Domain(Top);
                    break;
                case MayNeedCall:
                    return Domain(MayNeedCall); 
                    break;
                case NeedsCall:
                    return Domain(MayNeedCall); 
                    break;
                case DoesCall:
                    return Domain(DoesCall);
                    break;
                case Bottom: 
                    return Domain(DoesCall);
                    break; 
            }
            break; 
            
        case Bottom: 
            return Domain(E2); 
            break; 
    }
}


bool Domain::equal(Domain D1, Domain D2) { return D1.Value == D2.Value; }

void Domain::print(raw_ostream &O) {
  switch (Value) {
  case Top:
    O << "Top";
    break;
  case MayNeedCall:
    O << "MayNeedCall";
    break;
  case NeedsCall:
    O << "NeedsCall";
    break;
  case DoesCall:
    O << "DoesCall";
    break;
  case Bottom: 
    O << "Bottom";
    break; 
  }
}

raw_ostream &operator<<(raw_ostream &O, Domain V) {
  V.print(O);
  return O;
}

}; // namespace dataflow
