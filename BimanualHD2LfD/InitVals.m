numbOfVar  = 2;   %Number of input variables in the GMM
numGauss   = 3;   %Number of Gaussians
numOfDemos = 1;   %Number of demonstrations used to train the system
Data       = [];  %Initialize the Data matrix, used to store the training values


model.nbVar = numbOfVar+1; %Number of variables used in the GMM
model.nbStates = numGauss; %Number of Gaussians
