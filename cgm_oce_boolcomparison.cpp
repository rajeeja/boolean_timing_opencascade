#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepAlgoAPI_Cut.hxx>



int main(int argc, char **argv){
  testing::InitGoogleTest(&argc, argv);

  // create the first shape: a cube, a corner at the origin
  TopoDS_Solid box1 = (TopoDS_Solid) BRepPrimAPI_MakeBox(10.,10.,10.);
  ASSERT_FALSE(box1.IsNull());
  // we will create a lot of cylinder using input commands nxn cylinders will be subtracted from the original box
  // size of the cylinder and the move command will be computed accordingly.
  TopoDS_Solid cylinder = BRepPrimAPI_MakeCylinder(2.,10.);
  ASSERT_FALSE(cylinder.IsNull());
  // boolean cut
  TopoDS_Shape shp_result = BRepAlgoAPI_Cut(box1,cylinder);
  ASSERT_FALSE(shp_result.IsNull());

}
