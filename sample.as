# This is a comment
!echo Hello, World!

[win]
!echo Running on Windows
!dir

[linux]
!echo Running on Linux
!ls -l

# Conditional statement
if {OS} == "win"
!echo This is Windows
else
!echo This is not Windows
endif

# Loop
for i in 1 2 3
!echo Iteration {i}
endloop

# Function
function myfunc
!echo Inside function
endfunction

# Function call
&myfunc

# Error handling
error Something went wrong