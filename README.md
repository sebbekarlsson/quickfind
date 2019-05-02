# qf
> Simple, quick, yet powerful tool to find files based on the name & content.

## Usage
> To use quickfind:

    $ qf <name_query> <content_query>

> For example:

    $ qf .py generate_

    ./srcrender/venv/lib/python2.7/site-packages/numpy/core/generate_numpy_api.py:157
    ./srcrender/venv/lib/python2.7/site-packages/numpy/core/tests/test_ufunc.py:211
    ./srcrender/venv/lib/python2.7/site-packages/numpy/core/tests/test_numeric.py:1340
    ./srcrender/venv/lib/python2.7/site-packages/numpy/core/tests/test_ufunc.pyc:85
    ./srcrender/venv/lib/python2.7/site-packages/numpy/core/setup.py:403
    ./srcrender/venv/lib/python2.7/site-packages/numpy/random/setup.py:23

> The output is in the following format:

    <path>:<line_number>


> Both variables are fuzzy, `name_query` _and_ `content_query`.

## Install
> To install, simply clone down repository and run:

    $ sudo make install

## Why?
> The advantages of quickfind is that it does not try to be as smart as other
> search tools, it does one thing and it does it good.  
> Another advantage of quickfind is that the output is in vim-format (`path`:`line_number`),
> so it is really easy to open found files in vim if needed.
