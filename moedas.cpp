
#include <iostream>
#include <stdio.h>
#include "opencv/highgui.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv/cv.h>
#include "imagem_io.h"
#include "filtros.h"
#include "util.h"


using namespace std;
using namespace cv;

const int COIN_5(5);
const int COIN_10(10);
const int COIN_25(25);
const int COIN_50(50);
const int COIN_1000(100);

int coins() {
	for(int i = 1; i <= 8; i ++){
		Mat src, src_gray;
		
		/// Variavel auxiliar para obter o indice da imagem em string
		stringstream ss;
		ss << i;
		
		/// Realiza a leitura da imagem original e salva em src.
		src = imread("moedas" + ss.str() + ".png", cv::IMREAD_COLOR);

		/// Transforma imagem original em escala de cinza
		cvtColor(src, src_gray, CV_BGR2GRAY);

		/// Gaussian Blur para não encontrar falsos círculos
		GaussianBlur(src_gray, src_gray, Size(3, 3), 2, 2);

		/// Vetor que contera o circulos encontrados pelo algoritmo
		vector<Vec3f> circles;

		/// Aplicação do algoritmo hough para encontrar circulos
		HoughCircles(src_gray, circles, CV_HOUGH_GRADIENT, 1, 28, 140, 21.5, 28, 40 );
		
		/// vetor que contera os valores de cada moeda encontrada
		int values[circles.size()];
		
		/// Parametro necessario para salvar imagem com imwrite
		vector<int> compression_params;
		compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION); 
		compression_params.push_back(9);
		
		/// Contadores para o numero de moedas	
		int cont0 = 0;
		int cont5 = 0;
		int cont10 = 0;
		int cont25 = 0;
		int cont50 = 0;
		int cont100 = 0;

		/// Para cada circulo encontrado, obtem-o em uma imagem separada 
		/// e calcula o histograma deste
		for (int i = 0; (unsigned)i < circles.size(); i++) {
			/// Variavel auxiliar para obter o indice da moeda encontrada em string
			stringstream stream;
			stream << i;
			
			/// Ponteiro com valores do centro do circulo (x,y)
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			
			/// Impressao para saber a imagem e a moeda que está sendo computada
			cout << "Imagem " << ss.str() << "-Moeada " << stream.str() << endl;
			
			/// Variavel que recebe o raio do circulo encontrado
			double radius = (circles[i][2]);
			//cout << "Raio : " << radius << endl;

			/// Desenha o circulo interno (centro - verde) e externo (bordas - vermelho) na imagem de cada moeda
			circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
			circle(src, center, radius, Scalar(0, 0, 255), 3, 8, 0);
			
			/// Obtem imagem da moeda atual a partir da imagem original
			int x = circles[i][0]-circles[i][2] < 0 ? 0 : circles[i][0]-circles[i][2];
            int y = circles[i][1]-circles[i][2] < 0 ? 0 : circles[i][1]-circles[i][2];
            int height = y + 2*circles[i][2] > src.rows ? src.rows - x : 2*circles[i][2];
            int width  = x + 2*circles[i][2] > src.cols ? src.cols - x : 2*circles[i][2];
            Mat roi (src, Rect(x,y,width,height));
            
            /// Salva e mostra a imagem de cada moeda encontrada
            imwrite(ss.str() + "-moeda" + stream.str() + ".png",roi, compression_params);
            //namedWindow(stream.str());
            //imshow(stream.str(), roi);
            
            /// Vetor para armazenar os canais BGR da imagem da moeda
            vector<Mat> bgr_planes;
			split( roi, bgr_planes );

			/// Numero de níveis de cinza para o calculo do histograma
			int histSize = 256;

			/// Configura valores ( para (B,G,R) )
			float range[] = { 0, 256 } ;
			const float* histRange = { range };

			bool uniform = true; bool accumulate = false;

			/// Matrizes que irão armazenar os valores de histograma para B, G e R
			Mat b_hist, g_hist, r_hist;

			/// Calcula os histogramas:
			calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
			calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
			calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
		  
			/// Desenha o histograma para BGR
			int hist_w = 256; int hist_h = 256;
			int bin_w = cvRound( (double) hist_w/histSize );

			Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

			/// Normaliza os resultados para [ 0, histImage.rows ]
			normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
			normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
			normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

			/// Desenha cada canal do histograma
			for( int j = 1; j < histSize; j++ )
			{
			  line( histImage, Point( bin_w*(j-1), hist_h - cvRound(b_hist.at<float>(j-1)) ) ,
							   Point( bin_w*(j), hist_h - cvRound(b_hist.at<float>(j)) ),
							   Scalar( 255, 0, 0), 2, 8, 0  );
			  line( histImage, Point( bin_w*(j-1), hist_h - cvRound(g_hist.at<float>(j-1)) ) ,
							   Point( bin_w*(j), hist_h - cvRound(g_hist.at<float>(j)) ),
							   Scalar( 0, 255, 0), 2, 8, 0  );
			  line( histImage, Point( bin_w*(j-1), hist_h - cvRound(r_hist.at<float>(j-1)) ) ,
							   Point( bin_w*(j), hist_h - cvRound(r_hist.at<float>(j)) ),
							   Scalar( 0, 0, 255), 2, 8, 0  );							 
			}
		  
			/// Salva e mostra imagem do histograma
			imwrite(ss.str() + "-histograma" + stream.str() + ".png",histImage, compression_params);
			//namedWindow("window1 " + stream.str(), CV_WINDOW_AUTOSIZE );
			//imshow("window1 " + stream.str(), histImage );
		  
			/// Obtem a média do histograma em cada camada BGR
			//cout << "Media " << mean(b_hist)[0] << "-" << mean(g_hist)[0] << "-" << mean(r_hist)[0] << endl;
			double media_b = mean(b_hist)[0]; //8.7
			double media_g = mean(g_hist)[0]; //9.0
			double media_r = mean(r_hist)[0]; //9.0
		  
			values[i] = 0;
		 
			/// Compara os valores encontrados e dá o resultado para a moeda
			if(radius > 35.5 &&  radius < 39.31 && media_b > 5.35 && media_b < 5.79 && media_g > 5.57 && media_g < 5.96){
				values[i] = COIN_1000;
				cont100 ++;
				cout << values[i] << endl;
			}else if(radius > 30.62 && radius < 32.91 && media_b > 4.44 && media_b < 4.91 && media_g > 4.58 && media_g < 5.09){
				values[i] = COIN_50;
				cont50 ++;
				cout << values[i] << endl;
			}else if(radius > 29.53 && radius < 36.12 && media_b > 4.47 && media_b < 5.49 && media_g > 4.61 && media_g < 5.66){
				values[i] = COIN_25;
				cont25 ++;
				cout << values[i] << endl;
			}
			if(radius > 29.53 && radius < 31.85 && media_b > 4.04 && media_b < 4.91 && media_g > 4.18 && media_g < 5.09){
				if (values[i] == 100) { cont100 --; }
				else if (values[i] == 50) { cont50 --; }
				else if (values[i] == 25) { cont25 --; }
				values[i] = COIN_5;
				cont5 ++;
				cout << values[i] << endl;
			}else if(radius > 28.36 && radius < 30.71 && media_b > 3.3 && media_b < 4.46 && media_g > 3.4 && media_g < 4.64){
				if (values[i] == 100) { cont100--; }
				else if (values[i] == 50) { cont50--; }
				else if (values[i] == 25) { cont25--; }
				values[i] = COIN_10;
				cont10 ++;
				cout << values[i] << endl;
			}
		}
		
		/// Se alguma(s) moeda(s) não encaixar em nenhuma das condições anteriores, ela será não real
		cont0 = circles.size() - (cont100 + cont50 + cont25 + cont10 + cont5);
		
		/// Mostra os resultados parciais encontrados
		cout << "1,00 - " << cont100 << endl;
		cout << "0,50 - " << cont50 << endl;
		cout << "0,25 - " << cont25 << endl;
		cout << "0,10 - " << cont10 << endl;
		cout << "0,05 - " << cont5 << endl;
		cout << "Não Real - " << cont0 << endl;

		/// Mostra o resultado total encontrado
		cout << endl << "Resultado:" << endl;
		double result = 0;
		for (int i = 0; (unsigned)i < circles.size(); i++) {
			result += values[i];
			//cout << values[i] << endl;
			values[i] = 0;
		}
		cout << "O Resultado é : R$" << (result / 100) << endl << endl;


		/// Mostra imagem original
		//namedWindow("Count Coins" + ss.str(), CV_WINDOW_AUTOSIZE);
		//imshow("Count Coins" + ss.str(), src);
		//waitKey(0);
	}
    return 0;
}

int main(int argc, char** argv){
	coins();
	return 0;
}
