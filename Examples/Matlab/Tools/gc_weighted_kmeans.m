% gc_weighted_kmeans   Calculate weighted k-Means clustering using
%                      Gibou-Fedkiw algorithm.
%
% SYNOPSIS:
%  [c1 c2 iter] = gc_weighted_kmeans(image_in, lambda1, lambda2, conv, maxiter)
%
% EXAMPLE:
%  a = imread('somefile.tif');
%  [c1 c2 iter] = gc_weighted_kmeans(single(a), 10, 10);
%
% PARAMETERS:
%  image_in: Input image. 
%  lambda1: Lambda 1 weight parameter.
%  lambda1: Lambda 2 weight parameter.
%  conv: Convergence criterion. The minimization is stopped when the sum of
%    c1 and c2 deltas is less than this number.
%  maxiter: Maximum number of iterations.
%
%  c1: First mean value.
%  c2: Second mean value.
%  iter: Number of iterations performed.
%
% PARAMETERS:
%  image_in   SINGLE[] or DOUBLE[]
%  lambda1    DOUBLE
%  lambda2    DOUBLE
%  conv       DOUBLE
%  maxiter    DOUBLE
%
%  c1         DOUBLE
%  c2         DOUBLE
%  iter       DOUBLE
%
% DEFAULTS:
%  image_in   compulsory
%  lambda1    compulsory
%  lambda2    compulsory
%  conv       0.001
%  maxiter    30
%
% DESCRIPTION:
%  Calculates the weighted k-Means clustering (for 2 means) using the iterative 
%  algorithm proposed by Gibou and Fedkiw and returns the estimated mean values.
%
% LITERATURE:
%  [1] Frederic Gibou and Ronald Fedkiw. A fast hybrid k-means level set algorithm for 
%      segmentation. In 4th Annual Hawaii International Conference on Statistics and 
%      Mathematics, pages 281–291, 2002.
%
% Copyright (c) 2010 Centre for Biomedical Image Analysis
% Copyright (c) 2010 Ondrej Danek

function [c1 c2 iter] = gc_weighted_kmeans(img, lambda1, lambda2, conv, maxiter)

% Check input data type
if (isfloat(img) == false)
    error('Use only with SINGLE[] or DOUBLE[] images.');
end

% Check parameter count
if (nargin < 3)
    error('At least 4 parameters required.');
end

% Supply default parameters if necessary
if (nargin < 4)
    % Default convergence criterion
    conv = 0.001;    
end

if (nargin < 5)
    % Default maximum number of iterations
    maxiter = 30;
end

% Init c1 and c2
avg = mean(img(:));
c1 = avg - (avg - min(img(:))) / 2;
c2 = avg + (max(img(:)) - avg) / 2;

% Iterative algorithm
iter = 0;
old_c1 = -1;
old_c2 = -1;

while (abs(c1-old_c1) + abs(c2-old_c2) > conv) && (iter < maxiter)
    R = -lambda1 * (img - c1).^2 + lambda2 * (img - c2).^2;
    old_c1 = c1;
    old_c2 = c2;
    c1 = mean(img(R >= 0));
    c2 = mean(img(R < 0)); 
    iter = iter + 1;
end

c1 = double(c1);
c2 = double(c2);

end