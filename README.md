# OpenGL Visualization of mRNA Forming 3D Structure through Coarse-grain Simulation by SPQR
<a href="https://github.com/timkartar/cs596_rna_viz/graphs/contributors">
  <img src="https://contributors-img.web.app/image?repo=timkartar/cs596_rna_viz" />
</a>

## Project
One of the recent Kaggle challenges was to predict the degradation rates of nucleotides in an RNA molecule. At the time, messenger RNA (mRNA) vaccines were the most promising candidates for vaccines, but they have issues with degradation (stability). The cut at a nucleotide can make a molecule ineffective. The goal of the competition was to accurately predict the likelihood of each base degrading to see which candidates were most stable.

We think that the 3D structure will be informative about the degradation rates of RNA, but these molecules are generally large and difficult to simulate. Instead, we will make use of coarse-grain simulations to overcome this obstacle.

SPQR is a coarse-grain method shown to accurately predict secondary and tertiary structure. It represents each nucleotide as 2 points and focuses on modeling base-base and base-sugar interactions.

The goal of our project is to visualize the output from such simulation softwares such as SPQR on RNA molecules in parallel to assist in this process using OpenGL in C. 

![spqrrna](https://github.com/timkartar/cs596_rna_viz/blob/main/demo_gif/rna_animation_demo.gif?raw=true)*OpenGL visualization of an mRNA forming 3D structure through course grain simulation by SPQR*

Color Legend: `Red` - Ribose Sugar, `Green` - Phospate Residue, rest are RNA bases 'A','U','G','C'. 

In the simulation demo above we can clearly see secondary base-paired RNA structures forming. 

The demo was generated from 100 equally spaced snapshots of a coarse grain simulation by the SPQR-eMC simulation process.

The simulations are done following instructions given in the SPQR documentation <a href="https://github.com/timkartar/cs596_rna_viz/blob/main/spqr/doc/ug.pdf"> here </a>

## Run Instruction: 
1. Follow OS-specific compilation instruction for OpenGL C application given here to generate executables `rnav`,`rna_animation` . <br/>
2. To generate a static image from one PDB file in with one ATOM description per line: `./rnav <pdb_file_name>` <br/>
3. To generate a video simulation from multiple stepwise files: `./rna_animation <pdb_dir_name> <number of files>` . <br/>
- no `/` at the end of `<pdb_dir_name>` and files should be named as `init_XX.pdb` where `XX` ranges from 0 to `<nnumber of files> - 1`


![spqrrna](https://github.com/timkartar/cs596_rna_viz/blob/main/example_visualization.png?raw=true)*Static single stranded RNA visualization*



### Available Data
The input (training) data was a set of 3,000 RNA molecules with rates of degradation at each molcule.

Some of the notable fields are:
- id
- sequence
- 2D structure
- predicted loop type

## Acknowledgments 
We thank Prof. Aiichiro Nakano and CSCI 596 course organizers at University of Southern California for the opportunity, help, and resources to make this project possible. 

### References/Links
S. Poblete, S. Bottaro and G. Bussi, A nucleobase-centric coarse-grained model for structure prediction of RNA motifs. _Nucleic Acids Res._ 46(4), 1674-1683 (2018).  [Link] https://doi.org/10.1093/nar/gkx1269 

S. Bottaro, F. Di Palma and G. Bussi, The role of nucleobase interactions in RNA structure and dynamics. _Nucleic Acids Res._ 42(21), 13306-13314 (2014).

S. Poblete, S. Bottaro and G. Bussi, Effects and limitations of a nucleobase-driven backmapping procedure for nucleic acids using steered molecular dynamics. _Biophys. Biochem. Res. Comm._ 498 (2), 352-358 (2018).

Eterna platform. https://eternagame.org
