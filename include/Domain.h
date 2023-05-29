#ifndef DOMAIN_H
#define DOMAIN_H

#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace dataflow {

class Domain {
public:
  enum Element { Top, MayNeedCall, NeedsCall, DoesCall, Bottom };
  Domain();
  Domain(Element V);
  Element Value;

  static Domain* applyAllocate(Domain* D);
  static Domain* applyDeallocate(Domain* D);
  static Domain* applyReallocate(Domain* D);
  static Domain* applySafe(Domain* D); // TODO: consider removing this function entirely as it is handled by other parts of the code
  static Domain* applyUnsafe(Domain* D);
  static Domain join(Element E1, Element E2);

  void selfApplyAllocate();
  void selfApplyDeallocate();
  void selfApplyReallocate();
  void selfApplyUnsafe();
  
  static bool equal(Domain D1, Domain D2);
  void print(raw_ostream &O);
};

raw_ostream &operator<<(raw_ostream &O, Domain V);


} // namespace dataflow

#endif // DOMAIN_H
