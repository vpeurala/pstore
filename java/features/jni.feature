Feature: JNI
  In order to access P-Store with Java
  As a user
  I want to examine the output of Java example program

  Scenario: Write and read database
  When I run "java -Djava.library.path=. -cp . pstore.examples.PStore"
  Then P-Store should output
    """
    # Table: table
    # Column: Column1 (ID = 0, type = 1)
    col1.1
    col2.1
    col1.1
    col2.1
    # Column: Column2 (ID = 1, type = 1)
    col1.2
    col2.2
    col1.2
    col2.2
    # Column: Column3 (ID = 2, type = 1)
    col1.3
    col2.3
    col1.3
    col2.3

    """
