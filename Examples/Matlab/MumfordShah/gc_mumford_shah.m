% gc_mumford_shah   Calculate piecewise constant Mumford-Shah segmentation 
%                   via graph cuts.
%
% SYNOPSIS:
%  [seg energy iter c] = gc_mumford_shah(img, k, lambda, nb, log, conv, maxiter)
%
% EXAMPLE:
%  a = imread('somefile.tif');
%  an = gc_normalize_image(single(a));
%  [seg energy iter c] = gc_mumford_shah(an, 4, 10);
%
% PARAMETERS:
%  img: Input image. It is recommended to normalize the image using
%   gc_normalize_image function to better balance the regularization and
%   data terms of the functional. Smoothing using a gaussian filter 
%   is recommended prior to normalization and segmentation.
%  k: The number of segmentation classes, i.e., the number of possible
%   labels.
%  lambda: Data term weight. Greater than 0.
%  nb: Neighbourhood type. One of:
%    - for 2D: 'N4', 'N8', 'N16' and 'N32'
%    - for 3D: 'N6', 'N18', 'N26' and 'N98' (equivalent of N16 in 3D)
%  log: Enable/disable logging.
%  conv: Convergence criterion. The minimization is stopped when the sum of
%   deltas of the mean values is less than this number.
%  maxiter: Maximum number of iterations.
%
%  seg: Final labeling.
%  energy: Energy of the final labeling.
%  iter: Number of iterations performed.
%  c: Mean values of the partitions.
%
% PARAMETERS:
%  img        SINGLE[] or DOUBLE[]
%  k          DOUBLE
%  lambda     DOUBLE
%  nb         STRING
%  log        LOGICAL
%  conv       DOUBLE
%  maxiter    DOUBLE
%
%  seg        UINT8[]
%  energy     DOUBLE
%  iter       DOUBLE
%  c          DOUBLE[]
%
% DEFAULTS:
%  img        compulsory
%  k          compulsory
%  lambda     compulsory
%  nb         'N16' in 2D, 'N26' in 3D
%  log        false
%  conv       0.001
%  maxiter    30
%
% DESCRIPTION:
%  Computes the piecewise constant Mumford-Shah segmentation of an image 
%  with a given data term weight via graph-cut based optimization.
%
% NOTES:
%  The data type of the input image determines also the precision used
%  during the minimization. Images of constant intensity are not supported. 
%  If method returns a 'Convergence error' than your parameters are probably 
%  too strong or the number of labels is too small or too large.
%
% LITERATURE:
%  [1] Mumford, D., Shah, J.: Optimal approximations by piecewise smooth functions 359
%      360 and associated variational problems. Communications on Pure and Applied 
%      Mathematics. 42 (1989) 577-684
%  [2] Y. Boykov, O. Veksler, and R. Zabih. Fast approximate energy minimization via graph
%      cuts. IEEE Transactions on Pattern Analysis and Machine Intelligence, 23(11):1222-1239,
%      2001.
%  [2] O. Danek and P. Matula: Graph Cuts ans Approximation of the
%      Euclidean Metric on Anisotropic Grids. In: VISAPP 2010 International
%      Conference on Computer Vision Theory and Applications, Volume 2,
%      pages 68-73, 2010
%
% SEE ALSO:
%  gc_normalize_image
%
% Copyright (c) 2010 Centre for Biomedical Image Analysis
% Copyright (c) 2010 Ondrej Danek

function [seg energy iter c] = gc_mumford_shah(img, k, lambda, nb, log, conv, maxiter)

% Check input data type
if (isfloat(img) == false)
    error('Use only with SINGLE[] or DOUBLE[] images.');
end

% Check parameter count
if (nargin < 3)
    error('At least 3 parameters required.');
end

% Supply default parameters if necessary
if (nargin < 4)
    % Default neighbourhood size
    if (ndims(img) == 2)
        nb = 'N16';
    else
        nb = 'N26';
    end
end

if (nargin < 5)
    % Disable logging by default
    log = false;    
end

if (nargin < 6)
    % Default convergence criterion
    conv = 0.001;    
end

if (nargin < 7)
    % Default maximum number of iterations
    maxiter = 30;
end

[seg energy iter c] = GcMumfordShah(img, k, lambda, conv, maxiter, nb, log);

end
