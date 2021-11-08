using BenchmarkTools
using MAT
using SparseArrays
using Setfield
using Glob
include("MatrixMarket.jl")
function triangle_counting(A)
	sum(A .* ( A * A ))/6
end

function sequential_masked_triangle_counting( mtx )
	rows = rowvals(mtx)
	m, n = size(mtx)
	temp = 0
	for i = 1:m
    	for j in nzrange(mtx, i)
    		row = rows[j]
    		for k in nzrange(mtx, row)
    			for l in nzrange(mtx, i)
    				row1 = rows[l]
                    row2 = rows[k]
                    if row1 == row2
                    	temp+=1
                    end
				end
			end
		end 
	end
	return temp
end

function tamudata(group, data)
	url = "https://suitesparse-collection-website.herokuapp.com/mat/$group/$data.mat"
	save_path = download(url)
	vars = matread(save_path)
    A = vars["Problem"]["A"]
end

A = tamudata("SNAP", "com-Youtube")
#print(nnz(A))
print(sequential_masked_triangle_counting(A), "\n")
#=
datasets = glob("../data/com*.mtx") #if searching the working directory
for i in datasets
	M = mmread(i)
	print(nnz(M))
	#print(sequential_masked_triangle_counting(M), "\n")
end
=#
