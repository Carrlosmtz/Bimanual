model = init_GMM_kmeans(Data, model);   %Initialize the GMM components
model = EM_GMM(Data, model);            %Run EM algorithm to compute GMM components

%Run GMM with the trained data to get the expected values
[DataOut, SigmaOut] = GMR(model, Data(1:numbOfVar,:), 1:numbOfVar, numbOfVar+1);   

% Plot the expected values vs the "ideal" therapist's behaviour
figure
hold on
plot(DataOut)
plot(Data(numbOfVar+1,:))