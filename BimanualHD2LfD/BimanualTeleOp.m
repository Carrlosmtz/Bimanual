%% Clean scope
clear all
clc
addpath('m_fcts\');
%% Initial variables
InitVals
%% Load data
LoadData
%% Clean & process data
CleanAndProc
%% Get GMM & GMR
GMM_GMR
%% Simulation
Simulation