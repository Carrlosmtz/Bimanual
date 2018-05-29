% Data structure: A 4xN matrix where    Row 1) Patient's position
%                                       Row 2) Patient's velocity
%                                       Row 3) Force
%                                       Row 4) Therapist's position
%                                       N: Number of datapoints


for i = 1:numOfDemos
    eval(['load(''TrainingData\Training_',num2str(i),'.mat'')']);   % Load the training data
    Data = horzcat(Data,ans);                                       % Load the data in "Data" variable
    %NOTE: "ans" variable may not be the right name of the matrix after loading
    %       the data, plase change this name to the right one
end


% Notice that the previous code was developed in the case that we decide to
% add more than one demonstration, but given that we have a single
% demonstration it has the same function as:

% load('TrainingData\Training_1.mat')    % Load the training data
% Data = ans;                             % Load the data in "Data" variable