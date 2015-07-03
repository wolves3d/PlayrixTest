#ifndef __pch_h_included__
#define __pch_h_included__


using namespace std;
using namespace math;


// Forward declarations
class CGameNode;
class CTopGunGame;


// short type names
typedef unsigned int uint;
typedef vector <CGameNode *> NodeVector;


// Singletons
extern CTopGunGame * g_Game;


// Custom math
#define RADIAN_TO_DEGREE(X) (180.0f / math::PI * (X))

#define RND_FLOAT ((float)rand() / 32768.f)
#define RND_INTERVAL(FROM, TO) ((FROM) + (RND_FLOAT * ((TO) - (FROM))))



// general utils
unsigned int TimeInMilliseconds();


#endif // #ifndef __pch_h_included__