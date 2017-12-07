void hhist(std::string fileName){
	using namespace std;
	ifstream ifs(fileName.c_str());
	double pi;
	int I=0;
	TCanvas* c=new TCanvas("c","c",600,600);
	TH1D* hist=new TH1D("hist",fileName.c_str(),200,0,0.0003);
	while(ifs.is_open() && !ifs.eof() && I<100000){
		ifs >> pi;
		hist->Fill(pi);
//		std::cout << pi << std::endl;
		I++;
	}
	std::cout << I << " entries." << std::endl;
	hist->Draw();
/*	std::string pngFileName=fileName+".png";
	std::string pdfFileName=fileName+".pdf";
	c->SaveAs(pngFileName.c_str());
	c->SaveAs(pdfFileName.c_str());
*/	ifs.close();
}
