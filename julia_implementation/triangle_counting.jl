using BenchmarkTools
using MAT
using SparseArrays
using Setfield
using Glob
using Dates
import Base.Threads.@spawn
include("MatrixMarket.jl")

function linearSearch(arr, l, r, x)
    for i = l:r
        if (arr[i] == x)
            return i
        end
    end
    return -1;
end

function binarySearch(arr, l, r, x)
    while (l <= r)
        m = trunc(Int, l + (r - l) / 2)
        if (arr[m] == x)
            return m
        end
        if (arr[m] < x)
            l = m + 1
        else
            r = m - 1
        end
    end
    return -1
end



function triangle_counting(A)
	A .* ( A * A )
end


function obj_func(mtx2, lk, m)
	while(true)
		lock(lk[1])
		this_work = m[1]
		m[1] -= 5
		unlock(lk[1])
		if(this_work < 1)
		    break
		end
		for i = max(this_work-4,1):this_work
			for j = mtx2.colptr[i]:(mtx2.colptr[i+1]-1)
				row = mtx2.rowval[j]
				for k = mtx2.colptr[row]:(mtx2.colptr[row+1]-1)
					suc = -1
					if(mtx2.colptr[i+1]-mtx2.colptr[i] > 8)
						suc = binarySearch(mtx2.rowval, mtx2.colptr[i], mtx2.colptr[i+1]-1, mtx2.rowval[k])
					else
						suc = linearSearch(mtx2.rowval, mtx2.colptr[i], mtx2.colptr[i+1]-1, mtx2.rowval[k])
					end
					if(suc!=-1)
				    	mtx2.nzval[j]+=1
				    end
				end
			end
		end
	end 
end

function parallel_masked_triangle_counting( mtx )
	mtx2 = copy(mtx)
	m, n = size(mtx)
	mtx2  = @set mtx2.nzval = zeros(last(mtx2.colptr))
	lk = [Base.Threads.SpinLock()]
	
    n = Threads.nthreads()  # Number of tasks
    tasks = [m]
	@sync for i in 1:n
   		@spawn obj_func(mtx2, lk, tasks)
	end
	return mtx2
end


function sequential_masked_triangle_counting( mtx )
	mtx2 = copy(mtx)
	m, n = size(mtx)
	mtx2  = @set mtx2.nzval = zeros(last(mtx2.colptr))
	for i = 1:m
    	for j = mtx.colptr[i]:(mtx.colptr[i+1]-1)
    		row = mtx.rowval[j]
    		for k = mtx.colptr[row]:(mtx.colptr[row+1]-1)
    			suc = -1
    			if(mtx.colptr[i+1]-mtx.colptr[i] > 8)
    				suc = binarySearch(mtx.rowval, mtx.colptr[i], mtx.colptr[i+1]-1, mtx.rowval[k])
    			else
    				suc = linearSearch(mtx.rowval, mtx.colptr[i], mtx.colptr[i+1]-1, mtx.rowval[k])
    			end
    			if(suc!=-1)
                	mtx2.nzval[j]+=1
                end
			end
		end 
	end
	return mtx2
end

function tamudata(group, data)
	url = "https://suitesparse-collection-website.herokuapp.com/mat/$group/$data.mat"
	save_path = download(url)
	vars = matread(save_path)
    A = vars["Problem"]["A"]
end
full_mat = true

data = glob("../code/data/*.mtx")
file = Dates.format(now(), "d_m_Y_HH_MM_SS.txt")  
print(file,"\n")
print(Base.Threads.nthreads(),"\n")
for i in data
	mat = MatrixMarket.mmread(i, full_mat)
	#print(triangle_counting( mat ), "\n")
	#print(sequential_masked_triangle_counting( mat ))
	time_1 = @benchmark sequential_masked_triangle_counting( $mat) samples=1
	time_2 = @benchmark parallel_masked_triangle_counting( $mat ) samples=1
	open(file, "a") do io
		write(io, "$i seq: $time_1 parallel: $time_2\n")
    end
end
#=
#mat = MatrixMarket.mmread("/home/fdimitri/Parallel and Distributed Systems/Triagle_Counting/code/tests/data/test1.mtx",full_mat)
#mat = tamudata("Arenas", "email")
#time_1 = @benchmark parallel_masked_triangle_counting( $mat )
#time_2 = @benchmark sequential_masked_triangle_counting( $mat)
#print(time_1, " ", time_2, "\n")

if triangle_counting( mat ) == parallel_masked_triangle_counting( mat )
	@benchmark parallel_masked_triangle_counting( $mat )
else
	print("**OUTPUT IS INCORRECT! FIX CODE BEFORE BENCHMARKING!**")
end
=#
