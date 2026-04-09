%PRACTICA 1 - ESPAÑOL
clc;
close all;
clear all;
%ABRIR ARCHIVO 
archivo=fopen('C:\Users\karla\Downloads\TeoriadeLaInfo\P1\Libro_ESP.txt','r');
cadena = fscanf(archivo,'%c'); %guardar archivo en un vector 
numcaracteres=length(cadena);  %numero de caracteres del archivo 
%CIERRE DE ARCHIVO 
fclose(archivo); 

%vector con 27 letras de 27 del español mas el espacio, total 28 simbolos 
ABC=['a','b','c','d','e','f','g','h','i','j','k','l','m','n','ñ','o','p','q','r','s','t','u','v','w','x','y','z',' ']; 

%conteo de repeticiones por cada caracter 
ABC0=zeros(1,length(ABC)); %vector para conteo de repeticiones de c/letra 
cont=1; 

for x=1:length(ABC);
      rep=0; %variable donde se guarda el # de repeticiones de 'x' caracter 
    for y=1:length(cadena); 
        if cadena(y)==ABC(x); 
        rep=rep+1; 
        end 
    end 
    ABC0(cont)=rep; %guardo el # de repeticiones en el vector de conteo 
    cont=cont+1; 
    fprintf('\n%c repeticiones: %d',ABC(x),ABC0(x)); 
end 

p1=zeros(1,length(ABC0)); 
I1=zeros(1,length(ABC0)); 
  
for x=1:length(ABC0);   
p=(ABC0(x)./sum(ABC0));% cal de probabilidad 
p1(x)=p; 
I=log2(1/p);% cantidad de informacion por caracter 
if I==Inf  
I=0;       
end        
I1(x)=I; 
fprintf('\n%c:\trepeticiones %f\t\tP= %f\t\tI= %f bits \n',ABC(x),ABC0(x),p,I); 
end

%calculo de la entropia como fuente sin memoria 
h=zeros(1,length(p1)); 
for x=1:length(p1); 
    h(x)=p1(x)*I1(x); 
end 
H=sum(h); 
fprintf('\n ENTROPIA COMO FUENTE SIN MEMORIA= %f bits/simbolo\n',H); 

% Crear etiquetas de caracteres
etiquetas = cellstr(ABC.');   % convertir cada caracter en celda string
etiquetas{end} = '&';         % representar el espacio como '&'

% Gráfica de Probabilidades
figure;
bar(p1, 'FaceColor',[0 1 1]) 
set(gca,'XTick',1:length(ABC));
set(gca,'XTickLabel',etiquetas);
xlabel('Caracteres');
ylabel('Probabilidad');
title(['Karla Robert - Distribución de Probabilidad de Caracteres con Ñ']);
grid on;
% --- mostrar valores encima ---
for i=1:length(p1)
    text(i, p1(i)+0.001, sprintf('%.3f', p1(i)), ...
        'HorizontalAlignment','center','Rotation',90,'FontSize',7);
end

% Gráfica de Cantidad de Información
figure;
bar(I1, 'FaceColor',[1 0 1]) % color azulito
set(gca,'XTick',1:length(ABC));
set(gca,'XTickLabel',etiquetas);
xlabel('Caracteres');
ylabel('Información (bits)');
title(['Karla Robert - Cantidad de Información por Caracter con Ñ']);
grid on;
% --- mostrar valores encima ---
for i=1:length(I1)
    text(i, I1(i)+0.05, sprintf('%.2f', I1(i)), ...
        'HorizontalAlignment','center','Rotation',90,'FontSize',7);
end

% Ordenar probabilidades de mayor a menor y tomar los 10 primeros
[p1_ordenado, idx_orden] = sort(p1, 'descend');
top10_idx = idx_orden(1:10);
top10_vals = p1_ordenado(1:10);
top10_labels = etiquetas(top10_idx);

% Gráfica de barras Top-10
figure;
bar(top10_vals, 'FaceColor', [1 0 1])
set(gca, 'XTick', 1:10);
set(gca, 'XTickLabel', top10_labels);
xlabel('Caracteres');
ylabel('Probabilidad');
title('Karla Robert - 10 caracteres por probabilidad (mayor \rightarrow menor)');
grid on;
% --- mostrar valores encima ---
for i=1:length(top10_vals)
    text(i, top10_vals(i)+0.001, sprintf('%.3f', top10_vals(i)), ...
        'HorizontalAlignment','center','Rotation',90,'FontSize',8);
end

% ---- Relación Vocales vs Consonantes ----
vocales = ['a','e','i','o','u'];
idx_letras = 1:26;                 
idx_espacio = 27;           %Excluye al espacio de las letras

% Contar vocales
separarVocales = ismember(ABC(idx_letras), vocales);
num_vocales = sum(ABC0(idx_letras(separarVocales)));

% Contar consonantes
separarConso = ~separarVocales;
num_consonantes = sum(ABC0(idx_letras(separarConso)));
tasa_vc = num_vocales / num_consonantes;

fprintf('\n#Vocales: %d', num_vocales);
fprintf('\n#Consonantes: %d', num_consonantes);
fprintf('\nTasa Vocal/Consonante = %.6f\n', tasa_vc);

% ==== Gráfica de pastel Vocales vs Consonantes con etiquetas y estilo ====
total_letras = num_vocales + num_consonantes;
valores = [num_vocales num_consonantes];
porcentajes = (valores / total_letras) * 100;
etiquetas = {sprintf('Vocales (%.2f%%)', porcentajes(1)),sprintf('Consonantes (%.2f%%)', porcentajes(2)) };

figure;
h = pie(valores, etiquetas);
colores = [1 0 1;  % azul
           0 1 1]; % rosa
patchHandles = findobj(h, 'Type', 'Patch'); 
for k = 1:length(patchHandles)
    patchHandles(k).FaceColor = colores(k,:);
    patchHandles(k).EdgeColor = 'w';  % bordes blancos
end

title('Karla Robert - Gráfica de pastel de Vocales y Consonantes','FontSize', 12, 'FontWeight', 'bold');

% ==== Número total de palabras ====
palabras = strsplit(strtrim(cadena));  % Divide la cadena en palabras separadas por espacios

num_palabras = length(palabras); % Cuenta el número total de palabras

fprintf('\nNúmero total de palabras en el texto: %d\n', num_palabras);

% ==== Número promedio de letras por palabra ====
num_letras = sum(cadena ~= ' ');

% Calcular promedio
promedio_letras = num_letras / num_palabras;

fprintf('\nNúmero total de letras: %d', num_letras);
fprintf('\nNúmero total de palabras: %d', num_palabras);
fprintf('\nPromedio de letras por palabra: %.2f\n', promedio_letras);


%  ANALISIS PARA PAREJAS
cont2 = zeros(length(ABC), length(ABC));   % matriz de conteos para pares (x->y)

for ix = 1:length(ABC)
    for iy = 1:length(ABC)
        c = 0;
        for k = 1:(length(cadena)-1)
            if cadena(k)==ABC(ix) && cadena(k+1)==ABC(iy)
                c = c + 1;
            end
        end
        cont2(ix,iy) = c;
    end
end

% Probabilidades de pares
pares = cont2(:).';
p2 = pares / sum(pares);
I2 = zeros(size(p2));
nz = p2 > 0;
I2(nz) = log2(1./p2(nz));

%  ANALISIS PARA TERCIAS
cont3 = zeros(length(ABC), length(ABC), length(ABC)); % (x->y->z)

for ix = 1:length(ABC)
    for iy = 1:length(ABC)
        for iz = 1:length(ABC)
            c = 0;
            for k = 1:(length(cadena)-2)
                if cadena(k)==ABC(ix) && cadena(k+1)==ABC(iy) && cadena(k+2)==ABC(iz)
                    c = c + 1;
                end
            end
            cont3(ix,iy,iz) = c;
        end
    end
end

% Probabilidades de tercias
tercias = cont3(:).';
p3 = tercias / sum(tercias);
I3 = zeros(size(p3));
nz3 = p3 > 0;
I3(nz3) = log2(1./p3(nz3));

%  TOP-10 PAREJAS (dos caracteres)
conteo_parejas = cont2(:);
idx_no_cero_parejas = find(conteo_parejas > 0);
[parejas_ordenadas, orden_parejas] = sort(conteo_parejas(idx_no_cero_parejas), 'descend');
topN = min(10, numel(orden_parejas));
if topN > 0
    idx_lineal_top_parejas = idx_no_cero_parejas(orden_parejas(1:topN));
    [i2, j2] = ind2sub(size(cont2), idx_lineal_top_parejas);

    % Etiquetas: reemplaza espacio por '&'
    etiquetas_parejas = cell(topN,1);
    for t = 1:topN
        c1 = ABC(i2(t)); if c1 == ' ', c1 = '&'; end
        c2 = ABC(j2(t)); if c2 == ' ', c2 = '&'; end
        etiquetas_parejas{t} = sprintf('%c%c', c1, c2);
    end

    figure;
    bar(double(parejas_ordenadas(1:topN)), 'FaceColor', [1 0 1]); % rosa
    set(gca, 'XTick', 1:topN, 'XTickLabel', etiquetas_parejas);
    xlabel('Parejas');
    ylabel('Repeticiones');
    title('Karla Robert - 10 parejas más frecuentes');
    grid on;
    % --- mostrar valores encima ---
    for i=1:topN
        text(i, double(parejas_ordenadas(i))+1, sprintf('%d', double(parejas_ordenadas(i))), ...
            'HorizontalAlignment','center','Rotation',90,'FontSize',8);
    end
else
    warning('No hay parejas con apariciones > 0 para graficar.');
end

%  TOP-10 TERCIAS (tres caracteres)
conteo_tercias = cont3(:);
idx_no_cero_tercias = find(conteo_tercias > 0);   % solo tercias con al menos 1 aparición
[tercias_ordenadas, orden_tercias] = sort(conteo_tercias(idx_no_cero_tercias), 'descend');
topM = min(10, numel(orden_tercias));
if topM > 0
    idx_lineal_top_tercias = idx_no_cero_tercias(orden_tercias(1:topM));
    [i3, j3, k3] = ind2sub(size(cont3), idx_lineal_top_tercias);

    % Etiquetas: reemplaza espacio por '&'
    etiquetas_tercias = cell(topM,1);
    for t = 1:topM
        c1 = ABC(i3(t)); if c1 == ' ', c1 = '&'; end
        c2 = ABC(j3(t)); if c2 == ' ', c2 = '&'; end
        c3 = ABC(k3(t)); if c3 == ' ', c3 = '&'; end
        etiquetas_tercias{t} = sprintf('%c%c%c', c1, c2, c3);
    end

    figure;
    bar(double(tercias_ordenadas(1:topM)), 'FaceColor', [0 1 1]); % cyan
    set(gca, 'XTick', 1:topM, 'XTickLabel', etiquetas_tercias);
    xlabel('Tercias');
    ylabel('Repeticiones');
    title('Karla Robert - 10 tercias más frecuentes');
    grid on;
    % --- mostrar valores encima ---
    for i=1:topM
        text(i, double(tercias_ordenadas(i))+1, sprintf('%d', double(tercias_ordenadas(i))), ...
            'HorizontalAlignment','center','Rotation',90,'FontSize',8);
    end
else
    warning('No hay tercias con apariciones > 0 para graficar.');
end

%  MARKOV DE 1er ORDEN
pc1 = zeros(length(ABC), length(ABC));

for x1 = 1:length(ABC)
    for y1 = 1:length(ABC)
        if ABC0(y1) == 0
            pc1(x1,y1) = 0;
        else
            pc1(x1,y1) = cont2(x1,y1) / ABC0(y1); %suma todo el vector x
        end
    end
end

marcov1 = 0;
for y1 = 1:length(ABC)
    for x1 = 1:length(ABC)
        if pc1(x1,y1) ~= 0
            marcov1 = marcov1 + p1(y1) * pc1(x1,y1) * log2(1/pc1(x1,y1));
        end
    end
end
fprintf('\nENTROPIA CON MEMORIA (MARKOV 1°) = %f bits/símbolo\n', marcov1);

%  MARKOV DE 2do ORDEN
pc2 = zeros(length(ABC), length(ABC), length(ABC));

for x2 = 1:length(ABC)
    for y2 = 1:length(ABC)
        for z2 = 1:length(ABC)
            if cont2(y2,z2) == 0
                pc2(x2,y2,z2) = 0;
            else
                pc2(x2,y2,z2) = cont3(x2,y2,z2) / cont2(y2,z2); %suma todo el vector x,y,: 
            end
        end
    end
end

marcov2 = 0;
for x2 = 1:length(ABC)
    for y2 = 1:length(ABC)
        for z2 = 1:length(ABC)
            if pc2(x2,y2,z2) ~= 0 && pc1(y2,z2) ~= 0 && p1(z2) ~= 0
                marcov2 = marcov2 + ( p1(z2) * pc1(y2,z2) * pc2(x2,y2,z2) * log2(1/pc2(x2,y2,z2)) );
            end
        end
    end
end
fprintf('ENTROPIA CON MEMORIA (MARKOV 2°) = %f bits/símbolo\n', marcov2);

%  Comparativa %
figure;
hs = [H, marcov1, marcov2];
bar(hs, 'FaceColor', 	[1 0 1]); % morado
set(gca,'XTick',1:3,'XTickLabel',{'Sin memoria','Markov 1°','Markov 2°'});
ylabel('bits/símbolo');
title('Karla Robert - Comparación de entropías');
grid on;
% --- mostrar valores encima ---
for i=1:length(hs)
    text(i, hs(i)+0.05, sprintf('%.2f', hs(i)), ...
        'HorizontalAlignment','center','Rotation',90,'FontSize',9);
end
