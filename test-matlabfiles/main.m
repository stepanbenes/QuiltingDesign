% Script for checking C++ version outputs

clearvars;
path('functions',path);

%% Prepare inputs for C++ code
inFile  = '..\..\..\inputs\in2.jpg';
outFile = '..\test-iofiles\_specimen.tpg';
save_textified_image( inFile, outFile );

%% Load and dispaly processed images
nI = 16;
for iI = 1:nI
    
    inFile = sprintf( '..\\test-iofiles\\tile%i.tpg', iI-1 );    
%     inFile = sprintf( '..\\test-iofiles\\sample%i.tpg', iI-1 );
    img = load_textified_image( inFile );
    img = uint8(img);
    
    figure();
    imshow(img);
    
end;