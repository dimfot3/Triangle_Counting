using BenchmarkTools
using MAT
using SparseArrays
using Setfield
using Glob
using Dates
include("MatrixMarket.jl")

function triangle_counting(A)
	A .* ( A * A )
end

#with binary search
function parallel_masked_triangle_counting( mtx )
	mtx2 = copy(mtx)
	rows = rowvals(mtx)
	m, n = size(mtx)
	temp = 0
	Threads.@threads for i = 1:m
    	for j in nzrange(mtx, i)
    		mtx2.nzval[j] = 0
    		row = rows[j]
    		for k in nzrange(mtx, row)
                for l in nzrange(mtx, i)
                	if(rows[k] == rows[l])
                		mtx2.nzval[j]+=1
                	end
                end
			end
		end 
	end
	return mtx2
end

#with binary search
function sequential_masked_triangle_counting( mtx )
	mtx2 = copy(mtx)
	rows = rowvals(mtx)
	m, n = size(mtx)
	temp = 0
	for i = 1:m
    	for j in nzrange(mtx, i)
    		mtx2.nzval[j] = 0
    		row = rows[j]
    		for k in nzrange(mtx, row)
                for l in nzrange(mtx, i)
                	if(rows[k] == rows[l])
                		mtx2.nzval[j]+=1
                	end
                end
			end
		end 
	end
	return mtx2
end

full_mat = false
data = glob("../code/tests/data/*.mtx")
file = Dates.format(now(), "d_m_Y_HH_MM_SS.txt")  
print(file,"\n")
for i in data
	mat = MatrixMarket.mmread(i, full_mat)
	#print(triangle_counting( mat ), "\n")
	#print(sequential_masked_triangle_counting( mat ))
	time_1 = @benchmark sequential_masked_triangle_counting( $mat)
	time_2 = @benchmark parallel_masked_triangle_counting( $mat )
	open(file, "w") do io
		write(io, "$i seq: $time_1 parallel: $time_2\n")
    end
end

#mat = MatrixMarket.mmread("/home/fdimitri/Parallel and Distributed Systems/Triagle_Counting/code/tests/data/test1.mtx",full_mat)
#mat = tamudata("LAW", "dblp-2010")
#time_1 = @benchmark parallel_masked_triangle_counting( $mat )
#time_2 = @benchmark sequential_masked_triangle_counting( $mat)
#print(time_1, " ", time_2, "\n")
#=
if triangle_counting( mat ) == sequential_masked_triangle_counting( mat )
	print(parallel_masked_triangle_counting( mat))
else
	print(triangle_counting( mat ), "\n")
	print(sequential_masked_triangle_counting( mat ))
end
=#
