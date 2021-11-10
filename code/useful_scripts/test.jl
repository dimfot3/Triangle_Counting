using BenchmarkTools
using MAT
using SparseArrays
using Setfield
using Glob
include("MatrixMarket.jl")

function binarySearch(arr, l, r, x)
	while (l <= r)
        m =  trunc(Int, l + (r - l) / 2)
 
        if (arr[m] == x)
            return m
        end
 
        if (arr[m] < x) 
        	l = m + 1; 
    	else 
         	r = m - 1; 
        end
    end
    return -1;
end

function triangle_counting(A)
	A .* ( A * A )
end

#with binary search
function sequential_masked_triangle_counting( mtx )
	mtx = copy(mtx)
	rows = rowvals(mtx)
	m, n = size(mtx)
	temp = 0
	temp_arr = Array{Float64,1}(undef, nnz(mtx))
	for i = 1:nnz(mtx)
		temp_arr[i] = 0
	end
	for i = 1:m
    	for j in nzrange(mtx, i)
    		row = rows[j]
    		for k in nzrange(mtx, row)
    			row1 = rows[k]
                succ = binarySearch(mtx.rowval, mtx.colptr[i], mtx.colptr[i+1]-1, row1)
                if(succ!=-1)
                	temp_arr[j]+=1
                end
			end
		end 
	end
	mtx = @set mtx.nzval = temp_arr
	return mtx
end

function tamudata(group, data)
	url = "https://suitesparse-collection-website.herokuapp.com/mat/$group/$data.mat"
	save_path = download(url)
	vars = matread(save_path)
    A = vars["Problem"]["A"]
end

A_email = tamudata("Arenas", "email")

if triangle_counting( A_email ) == sequential_masked_triangle_counting( A_email )
	@benchmark sequential_masked_triangle_counting( $A_email )
else
	print("**OUTPUT IS INCORRECT! FIX CODE BEFORE BENCHMARKING!**\n")
end

