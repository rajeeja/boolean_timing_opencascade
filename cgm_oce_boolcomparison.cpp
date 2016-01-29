#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <gp_Trsf.hxx>

#include <gtest/gtest.h>
#include <vector>

#include <BRepTools.hxx>


TEST(BRepAlgoAPITestSuite, testCutCylSphere){

    // create the first shape: a cube, a corner at the origin
    TopoDS_Shape box1 = BRepPrimAPI_MakeBox(1.,1.,1.).Shape();
    ASSERT_FALSE(box1.IsNull());
    // we will create a lot of cylinder using input commands nxn cylinders will be subtracted from the original box
    // size of the cylinder and the move command will be computed accordingly.
    int n = 8;
    int num_cylinders = n*n;
    double rad = 1.0/(3.0*n);
    double pitch = 1.0/(n);
    double dx=0, dy=0, dz=0;

    std::vector<TopoDS_Shape> cylinder;
    cylinder.resize(num_cylinders);
    gp_Trsf GP;

    for(int i=0; i<n; i++){
            dx = pitch*i + rad + pitch/(2.0*n);
            for (int j =0; j <n; j++){
                    dy = pitch*j + rad + pitch/(2.0*n);

                    cylinder[n*i+j] = BRepPrimAPI_MakeCylinder(rad,1).Shape();
                    ASSERT_FALSE(cylinder[n*i+j].IsNull());
                    gp_Vec aVec(dx, dy, dz);
                    GP.SetTranslation(aVec); // Change the translation to be non-zero and boolean operations will start to work.

                    BRepBuilderAPI_Transform mytransform(cylinder[n*i+j], GP);
                    mytransform.Build();
                    cylinder[n*i+j] = mytransform.Shape();
                }
        }

    std::cout << "Boolean Cut Starts "  << " "  << std::endl;

    // boolean cut
    TopoDS_Shape shp_result = box1;
    for(int i=0; i<num_cylinders;i++){
            BRepAlgoAPI_Cut bcut(shp_result, cylinder[i]);
            shp_result = bcut.Shape();
            ASSERT_FALSE(shp_result.IsNull());
        }

    std::cout << "Boolean Cut Done "  << std::endl;


    BRepTools::Write(shp_result, "shape.brep");


}

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
