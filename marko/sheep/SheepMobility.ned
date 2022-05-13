package mysterio.marko.sheep;

import inet.mobility.base.MovingMobilityBase;

simple SheepMobility extends MovingMobilityBase {
    parameters:
        double initialX @unit(m) = default(uniform(constraintAreaMinX, constraintAreaMaxX));
        double initialY @unit(m) = default(uniform(constraintAreaMinY, constraintAreaMaxY));
        double initialZ @unit(m) = default(1m);
        double initialMovementHeading @unit(deg) = default(uniform(0deg, 360deg)); // initial heading
        double initialMovementElevation @unit(deg) = default(0deg); // initial elevation
        bool initFromDisplayString = default(true); // enables one time initialization from the subject module's display string
		volatile double waitTime @unit(s) = default(0s); // wait time between reaching a target and choosing a new one
        volatile double changeInterval @unit(s); // frequency of changing speed and angle (can be random)
        volatile double angleDelta @unit(deg); // rotate velocity vector by this much around the rotation axis defined by the rotationAxisAngle parameter (can be random)
        volatile double rotationAxisAngle @unit(deg) = default(0deg); // defines a vector in the plane perpendicular to the velocity, where 0 deg means the Z axis when bank is zero (can be random)
        volatile double speed @unit(mps) = default(50mps); //uniform(40mps,60mps)); // speed (can be random, updated every changeInterval)
        @class(SheepMobility);
}

