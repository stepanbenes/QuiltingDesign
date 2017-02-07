function save_textified_image( inFile, outFile )
% SAVE_TEXTIFIED_IMAGE opens standard image file and produces its textified
% version. Note that ordering of entries in outFile respects standard
% ordering in C/C++, not the Matlab colon operator.

rawImg = imread(inFile);

iFile = fopen( outFile, 'w+' );
fprintf( iFile, '%i\n', size(rawImg) );
fprintf( iFile, '\n' );
for i = 1:size(rawImg,1)
    for j = 1:size(rawImg,2)
        for k = 1:size(rawImg,3)
            fprintf( iFile, '%i\n', rawImg(i,j,k) );
        end;
    end;
end;
fclose(iFile);

end