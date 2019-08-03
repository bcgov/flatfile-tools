echo "starting bash_profile.."
# make a folder in the appropriate temporary location (ensure it's deleted before logff)
cd /cygdrive/c/Users/$USER/AppData/
mkdir -p tmp

# navigate to the bin folder
cd /cygdrive/r/$USER/bin/

# set up paths for required tools
alias vim="/cygdrive/c/Program\ Files\ \(x86\)/Vim/vim81/vim.exe"
R_FILE=/cygdrive/c/Program\ Files/R/R-3.5.2/bin/Rscript.exe
if ! test -f "$R_FILE"; then
 alias Rscript="/cygdrive/c/Program\ Files/R/R-3.6.1/bin/Rscript.exe"
else
  alias Rscript="/cygdrive/c/Program\ Files/R/R-3.5.2/bin/Rscript.exe"
fi
 
alias python="/cygdrive/c/Program\ Files/Python35/python/" #alias python="/cygdrive/c/Python27/python.exe"
export PATH="$PATH:/cygdrive/r/$USER/bin"

# test if programs built
FILE=/cygdrive/r/$USER/bin/csv_slice.exe
if ! test -f "$FILE"; then
  echo "setting up command line utilities.."
  ./setup.sh
else
  echo "command line utils already set up.."
fi

# go to the temp folder (make sure to clean it up after)
cd /cygdrive/c/Users/$USER/AppData/tmp/
