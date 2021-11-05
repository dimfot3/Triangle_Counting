while IFS=" " read -r group name
do
	wget "https://suitesparse-collection-website.herokuapp.com/MM/$group/$name.tar.gz"
    tar -xvf "$name.tar.gz"
    mv "$name/$name.mtx" ../data/
    rm -r $name
    rm $name.tar.gz
done < ../data/datasets.txt
