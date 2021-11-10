 Georgios Georgiou - Γεώργιος Γεωργίου - sdi1800220 - 1115201800220
 Panagiotis Mavrommatis - Παναγιώτης Μαυρομμάτης - sdi1800115 - 1115201800115

GitHub Link: https://github.com/blackeye18/c-_Nearest-neighbors-and-Clustering

Τιτλος: Finding k-nearest neighours using LSH and Hypercube Algorithms and Clustering using kmeans++ and either Lloyds, LSH or Hypercube

Περιγραφή: Στο Α κομμάτι της εργασίας, βρίσκουμε τους κ κοντινότερους γείτονες καθώς και τους γείτονες σε απόσταση R, χρησιμοποιώντας είτε τον αλγόριθμο LSH είτε τον αλγόριθμο Hypercube. Για να γίνει σύγκριση αποτελεσμάτων, υπολογίζουμε και σειριακά τους κοντινότερους γείτονες.
Στο Β κομμάτι της εργασίας κάνουμε clustering, παίρνοντας τα αρχικά centroids με την βοήθεια του αλγόριθμου kmeans++. Σε δεύτερη φάση, χρησιμοποιούμε είτε τον αλγόριθμο Lloyds, είτε κάνουμε radius search στο LSH ή στο Hypercube, για να υπολογίσουμε τα νέα, καλύτερα, centroids. Επιπλέον με την χρήση της silhouette βλέπουμε κατά πόσο καλό είναι το clustering που κάναμε.

Για την μεταγλώττιση:
Κάθε υποπρόγραμμα έχει το δικό του makefile και τρέχει ανεξάρτητα, οπότε η μεταγλώττιση γίνεται γράφοντας την εντολή make στον αντίστοιχο υποκαταλογο. Μπορεί να γίνει απευθείας μεταγλώττιση όλων των υποπρογραμμάτων γράφοντας make στον αρχικό κατάλογο.

Για να τρέξει το πρόγραμμα: 
Σε κάθε υποπρόγραμμα υπάρχει ο φάκελος build, στον οποίο "βγάνουν" τα εκτελέσιμα αρχεία.

Για το LSH:
./lsh 
οπού ο χρήστης δίνει τις παραμέτρους μέσα από το πρόγραμμα
./lsh –i input_small_id –q query_small_id -ο out -Ν 5 -R 1300
όπου το k και το L παίρνουν default  τιμές
./lsh –i input_small_id –q query_small_id -k 7 -L 6 -ο out -Ν 5 -R 1300
όπου όλοι οι παράμετροι δίνονται απο την γραμμή εντολών

Για το Hypercube:
./cube 
οπού ο χρήστης δίνει τις παραμέτρους μέσα από το πρόγραμμα
./cube –i input_small_id –q query_small_id -ο out
όπου οι υπολοιποι παράμετροι παίρνουν default τιμές
./cube –i input_small_id –q query_small_id –k 7 -M 12 -probes 4 -ο out -Ν 5 -R 1300
όπου όλοι οι παράμετροι δίνονται απο την γραμμή εντολών

Για το  cluster η εντολή για να τρέξει το προγραμμα είναι οι εξής:
./cluster -i input_small_id -c cluster.conf -o out -complete -m LSH
./cluster -i input_small_id -c cluster.conf -o out -m LSH

