function outImg = load_textified_image( inFile )
% LOAD_TEXTIFIED_IMAGE opens the textified version of an image.

raw = dlmread( inFile );

outImg = zeros(raw(1:3)');
counter = 3;
for i = 1:size(outImg,1)
    for j = 1:size(outImg,2)
        for k = 1:size(outImg,3)
            counter = counter + 1;
            outImg(i,j,k) = raw(counter);
        end
    end
end;
    
end    