#include "seal/seal.h"
#include "arithmetic.h"
#include "fhebuilder.h"

using namespace seal;
using namespace fhe;

int main() {
	EncryptionParameters parms(scheme_type::ckks);
	factorial(2);
	FHEBuilder fhe;
	cout << "INSTALL SUCCESS!" << endl;
}