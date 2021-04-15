


#include <UT/UT_DSOVersion.h>
//#include <RE/RE_EGLServer.h>


#include <UT/UT_Math.h>
#include <UT/UT_Interrupt.h>
#include <GU/GU_Detail.h>
#include <GU/GU_PrimPoly.h>
#include <GU/GU_PrimMesh.h>
#include <CH/CH_LocalVariable.h>
#include <PRM/PRM_Include.h>
#include <PRM/PRM_SpareData.h>
#include <OP/OP_Operator.h>
#include <OP/OP_OperatorTable.h>


#include <limits.h>
#include "LSYSTEMPlugin.h"
using namespace HDK_Sample;

//
// Help is stored in a "wiki" style text file. 
//
// See the sample_install.sh file for an example.
//
// NOTE : Follow this tutorial if you have any problems setting up your visual studio 2008 for Houdini 
//  http://www.apileofgrains.nl/setting-up-the-hdk-for-houdini-12-with-visual-studio-2008/


///
/// newSopOperator is the hook that Houdini grabs from this dll
/// and invokes to register the SOP.  In this case we add ourselves
/// to the specified operator table.
///
void
newSopOperator(OP_OperatorTable *table)
{
    table->addOperator(
	    new OP_Operator("CusLsystem",			// Internal name
			    "MyLsystem",			// UI name
			     SOP_Lsystem::myConstructor,	// How to build the SOP
			     SOP_Lsystem::myTemplateList,	// My parameters
			     0,				// Min # of sources
			     0,				// Max # of sources
			     SOP_Lsystem::myVariables,	// Local variables
			     OP_FLAG_GENERATOR)		// Flag it as generator
	    );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//PUT YOUR CODE HERE
//You need to declare your parameters here
//Example to declare a variable for angle you can do like this :
static PRM_Name	iterationsName("iterations", "Iterations");
static PRM_Name	      stepName("step",       "Step");
static PRM_Name	     angleName("angle", "Angle");
static PRM_Name	   grammarName("grammar",    "Grammar");

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//				                ^^^^^^^^    ^^^^^^^^^^^^^^^
//				                internal    descriptive version


// PUT YOUR CODE HERE
// You need to setup the initial/default values for your parameters here
// For example : If you are declaring the inital value for the angle parameter
static PRM_Default iterationsDefault(1.0);
static PRM_Default       stepDefault(1.0);
static PRM_Default      angleDefault(22.5);
static PRM_Default    grammarDefault(0, "F\nF->F[+F]F[-F]");

static PRM_Range iterationsRange(PRM_RANGE_RESTRICTED, 1, PRM_RANGE_UI, 10);
static PRM_Range       stepRange(PRM_RANGE_RESTRICTED, 0.00001, PRM_RANGE_UI, 5.0);
static PRM_Range      angleRange(PRM_RANGE_RESTRICTED, 0.0, PRM_RANGE_UI, 360.0);

////////////////////////////////////////////////////////////////////////////////////////

PRM_Template
SOP_Lsystem::myTemplateList[] = {
	// PUT YOUR CODE HERE
	// You now need to fill this template with your parameter name and their default value
	PRM_Template(PRM_INT,    PRM_Template::PRM_EXPORT_MIN, 1, &iterationsName, &iterationsDefault, 0, &iterationsRange),
	PRM_Template(PRM_FLT,    PRM_Template::PRM_EXPORT_MIN, 1, &stepName,       &stepDefault,       0, &stepRange),
	PRM_Template(PRM_FLT,    PRM_Template::PRM_EXPORT_MIN, 1, &angleName,      &angleDefault,      0, &angleRange),
	PRM_Template(PRM_STRING, PRM_Template::PRM_EXPORT_MIN, 1, &grammarName,    &grammarDefault),
	/////////////////////////////////////////////////////////////////////////////////////////
    PRM_Template()
};


// Here's how we define local variables for the SOP.
enum {
	VAR_PT,		// Point number of the star
	VAR_NPT		// Number of points in the star
};

CH_LocalVariable
SOP_Lsystem::myVariables[] = {
    { "PT",	VAR_PT, 0 },		// The table provides a mapping
    { "NPT",	VAR_NPT, 0 },		// from text string to integer token
    { 0, 0, 0 },
};

bool
SOP_Lsystem::evalVariableValue(fpreal &val, int index, int thread)
{
    // myCurrPoint will be negative when we're not cooking so only try to
    // handle the local variables when we have a valid myCurrPoint index.
    if (myCurrPoint >= 0)
    {
		// Note that "gdp" may be null here, so we do the safe thing
		// and cache values we are interested in.
		switch (index)
		{
		case VAR_PT:
			val = (fpreal) myCurrPoint;
			return true;
		case VAR_NPT:
			val = (fpreal) myTotalPoints;
			return true;
		default:
			/* do nothing */;
		}
    }
    // Not one of our variables, must delegate to the base class.
    return SOP_Node::evalVariableValue(val, index, thread);
}

OP_Node *
SOP_Lsystem::myConstructor(OP_Network *net, const char *name, OP_Operator *op)
{
    return new SOP_Lsystem(net, name, op);
}

SOP_Lsystem::SOP_Lsystem(OP_Network *net, const char *name, OP_Operator *op)
	: SOP_Node(net, name, op)
{
    myCurrPoint = -1;	// To prevent garbage values from being returned
}

SOP_Lsystem::~SOP_Lsystem() {}

unsigned
SOP_Lsystem::disableParms()
{
    return 0;
}

OP_ERROR
SOP_Lsystem::cookMySop(OP_Context &context)
{
	fpreal now = context.getTime();

	// PUT YOUR CODE HERE
	// Decare the necessary variables and get always keep getting the current value in the node
	// For example to always get the current angle thats set in the node, you need to :
	//    NOTE : ANGLE is a function that you need to use and it is declared in the header file to update your values instantly while cooking 
	int iters;
	float step;
	float angle;

	iters = ITERS(now);
	step  = STEP(now);
	angle = ANGLE(now);
	std::string grammar = GRAMMAR(now).toStdString();

    ///////////////////////////////////////////////////////////////////
	LSystem myplant;




	///////////////////////////////////////////////////////////////////////////

	//PUT YOUR CODE HERE
	// Next you need to call your Lystem cpp functions 
	// Below is an example , you need to call the same functions based on the variables you declare
    myplant.loadProgramFromString(grammar);  // TODO change
    myplant.setDefaultStep(step);
    myplant.setDefaultAngle(angle);


	///////////////////////////////////////////////////////////////////////////////

	// PUT YOUR CODE HERE
	// You the need to call the below function for all the genrations, so that the end points points will be
	// stored in the branches vector, you need to declare them first

	//for (int i = 0; i < generations ; i++)
	//{
	//	  myplant.process(i, branches);
	//}

	// TODO what - dont do that ^
	std::vector<LSystem::Branch> branches = std::vector<LSystem::Branch>();
	myplant.process(iters, branches);



	///////////////////////////////////////////////////////////////////////////////////


	// Now that you have all the branches, which are the start and end point of each point, its time to render 
	// these branches into Houdini 
    

	// PUT YOUR CODE HERE
	// Declare all the necessary variables for drawing cylinders for each branch 
    float		 rad, tx, ty, tz;
    int			 divisions, plane;
    int			 xcoord = 0, ycoord = 1, zcoord = 2;
    float		 tmp;
    UT_Vector4		 pos;
    GU_PrimPoly		*poly;
    int				 i;
    UT_Interrupt	*boss;

    // Since we don't have inputs, we don't need to lock them.

    divisions  = 10.0;	//5;// We need twice our divisions of points
    myTotalPoints = branches.size() * divisions;	// Set the NPT local variable value
    myCurrPoint   = 0;			// Initialize the PT local variable



    // Check to see that there hasn't been a critical error in cooking the SOP.
    if (error() < UT_ERROR_ABORT)
    {
		boss = UTgetInterrupt();
		//if (divisions < 4)
		//{
		//	// With the range restriction we have on the divisions, this
		//	//	is actually impossible, but it shows how to add an error
		//	//	message or warning to the SOP.
		//	addWarning(SOP_MESSAGE, "Invalid divisions");
		//	divisions = 4;
		//}
		gdp->clearAndDestroy();

		// Start the interrupt server
		if (boss->opStart("Building LSYSTEM"))
		{
			// PUT YOUR CODE HERE
			// Build a polygon
			// You need to build your cylinders inside Houdini from here
			// TIPS:
			// Use GU_PrimPoly poly = GU_PrimPoly::build(see what values it can take)
			// Also use GA_Offset ptoff = poly->getPointOffset()
			// and gdp->setPos3(ptoff,YOUR_POSITION_VECTOR) to build geometry.

			// maybe use either GU_Detail::clearAndDestroy or GEO_Detail::stashAll/GEO_Detail::destroyStashed
		
			// Make a cylinder for each branch
			for (int i = 0; i < branches.size(); i++) {
				// convert boss to UT_AutoInterrupt
				//if (boss->wasInterrupted())
				//	break;
				UT_Vector3 start;
				start(0) = branches.at(i).first[0];
				start(1) = branches.at(i).first[2];
				start(2) = branches.at(i).first[1];
				
				UT_Vector3 end;
				end(0) = branches.at(i).second[0];
				end(1) = branches.at(i).second[2];
				end(2) = branches.at(i).second[1];

				//GU_PrimMesh* mesh = GU_PrimMesh::build(gdp, 2, 10, GEO_SurfaceType::GEO_PATCH_QUADS, 0, 1);
				//mesh->getPointOffset(j);
				std::vector<UT_Vector3> startCircle = std::vector<UT_Vector3>();
				std::vector<UT_Vector3> endCircle = std::vector<UT_Vector3>();

				// Calculate points
				UT_Matrix3 transform = UT_Matrix3(1.0);
				transform.lookat(end, start);
				float ang = M_PI * 2 / divisions;

				for (int j = 0; j < divisions; j++) {
					// Get point along a circle
					UT_Vector3 p;
					p(0) = 0.1 * cos(ang * j);
					p(1) = 0.1 * sin(ang * j);
					p(2) = 0.0;
					// rotate to match the branch
					p = rowVecMult(p, transform); //colVecMult(transform, p));

					startCircle.push_back(start + p);
					endCircle.push_back(end + p);
				}

				// Load points
				GU_PrimPoly* polyStart = GU_PrimPoly::build(gdp, divisions, GU_POLY_CLOSED);
				GU_PrimPoly* polyEnd = GU_PrimPoly::build(gdp, divisions, GU_POLY_CLOSED);

				for (int j = 0; j < divisions; j++) {
					GA_Offset ptoffStart = polyStart->getPointOffset(j);
					gdp->setPos3(ptoffStart, startCircle.at(j));

					GA_Offset ptoffEnd = polyEnd->getPointOffset(j);
					gdp->setPos3(ptoffEnd, endCircle.at(j));

					// Set individual rectangle faces
					GU_PrimPoly* polyRect = GU_PrimPoly::build(gdp, 4, GU_POLY_CLOSED);

					for (int k = 0; k < 2; k++) {
						GA_Offset ptoffRect = polyRect->getPointOffset(k);
						gdp->setPos3(ptoffRect, startCircle.at((j + k) % divisions));
					}

					for (int k = 0; k < 2; k++) {
						GA_Offset ptoffRect = polyRect->getPointOffset(k + 2);
						gdp->setPos3(ptoffRect, endCircle.at((j + 1 - k) % divisions));
					}
				}
			}



			////////////////////////////////////////////////////////////////////////////////////////////

			// Highlight the star which we have just generated.  This routine
			// call clears any currently highlighted geometry, and then it
			// highlights every primitive for this SOP. 
			select(GU_SPrimitive);
		}

		// Tell the interrupt server that we've completed. Must do this
		// regardless of what opStart() returns.
		boss->opEnd();
    }

    myCurrPoint = -1;
    return error();
}

