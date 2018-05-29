% Data structure: A 4xN matrix where    Row 1) Patient's position
%                                       Row 2) Patient's velocity
%                                       Row 3) Force
%                                       Row 4) Therapist's position
%                                       N: Number of datapoints

load('SimulatedData\Simulation_1.mat') % Load the training data
SimData = ans;                          % Load the data in "SimData" variable
%NOTE: "ans" variable may not be the right name of the matrix after loading
%       the data, plase change this name to the right one


%Run GMR to get expected values given non-demonstrated inputs
[DataOut, SigmaOut] = GMR(model, SimData(1:numbOfVar,:), 1:numbOfVar, numbOfVar+1); 

% Plot the expected values vs the "ideal" therapist's behaviour
figure
hold on
plot(DataOut)
plot(SimData(numbOfVar+1,:))
