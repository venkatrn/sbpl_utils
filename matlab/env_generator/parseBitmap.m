function map = parseBitmap(image)
  if size(image,3)>1
    grayimage = rgb2gray(image);
  else 
    grayimage = image;
  end
  map = zeros(size(grayimage));
  grayimage = (255-grayimage);
  map = grayimage;
  map = flipud(map)';
