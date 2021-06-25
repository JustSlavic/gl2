#ifndef GL2_SRC_PHYSICS_SIMULATION_2D_MODEL_HPP
#define GL2_SRC_PHYSICS_SIMULATION_2D_MODEL_HPP

#include <physics/rigid_body.hpp>
#include <vector>


namespace physics_simulation_2d {


struct model {
    std::vector<physics::static_segment_2d> static_stuff;
    std::vector<physics::dynamic_body_2d> dynamic_stuff;

    void draw();
};


} // physics_simulation_2d


#endif // GL2_SRC_PHYSICS_SIMULATION_2D_MODEL_HPP
