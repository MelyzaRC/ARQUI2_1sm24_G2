import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { map } from 'rxjs/operators';

@Injectable({
  providedIn: 'root'
})
export class ApiService {

  private urlApi = 'http://3.147.69.159:3010';

  header: HttpHeaders;

  constructor(
    private http: HttpClient
  ) { }

  addTemperatura(data:any){
    return this.http.post(`${this.urlApi}/addTemperatura`, data ,{ headers: this.header})
    .pipe(map(data => data));
  }

  addHumedad(data:any){
    return this.http.post(`${this.urlApi}/addHumedad`, data ,{ headers: this.header})
    .pipe(map(data => data));
  }

  addIluminacion(data:any){
    return this.http.post(`${this.urlApi}/addIluminacion`, data ,{ headers: this.header})
    .pipe(map(data => data));
  }

  addDistancia(data:any){
    return this.http.post(`${this.urlApi}/addDistancia`, data ,{ headers: this.header})
    .pipe(map(data => data));
  }

  addAire(data:any){
    return this.http.post(`${this.urlApi}/addAire`, data ,{ headers: this.header})
    .pipe(map(data => data));
  }

}
