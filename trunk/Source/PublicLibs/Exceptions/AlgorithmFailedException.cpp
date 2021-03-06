/* AlgorithmFailedException.cpp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 04/09/2017
 * Last Modified    : 04/09/2017
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "PublicLibs/CompilerSettings.h"
#include "AlgorithmFailedException.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const char AlgorithmFailedException::TYPENAME[] = "AlgorithmFailedException";
ExceptionFactoryT<AlgorithmFailedException> AlgorithmFailedException_Instance;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
