// IFJ20: inputX functions

package main

func main() {
  print("Zadej retezec: ")
  s := ""
  i := 0
  f := 0.0
  err := 0
  s, err = inputs()
  print("STRING=", s, ". CHYBA=", err, "\n")

  print("Zadej cislo: ")
  i, err = inputi()
  print("INT=", i, ". CHYBA=", err, "\n")

  print("Zadej desetinne cislo: ")
  f, err = inputf()
  print("FLOAT64=", f, ". CHYBA=", err, "\n")  // na stdout vytisknuto (presmerovano do input.out) instrukci WRITE, kde je drobna nekompatibilita s Go, ktere tiskne exponent hexadecimalni reprezentace pomoci dvou cislic (tj. s pripadnou prebytecnou nulou)
  
}
