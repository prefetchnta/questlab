#include "easypr.h"
#include "easypr/util/switch.hpp"

#include "accuracy.hpp"
#include "chars.hpp"
#include "plate.hpp"

// %OPENCV%\x86\vc12\lib opencv_world300d.lib;

namespace easypr {

namespace demo {

// interactions

int accuracyTestMain() {
  std::shared_ptr<easypr::Kv> kv(new easypr::Kv);
  kv->load("source/easypr/resources/text/chinese_mapping");

  bool isExit = false;
  while (!isExit) {
    easypr::Utils::print_file_lines("source/easypr/resources/text/batch_test_menu");
    std::cout << kv->get("make_a_choice") << ":";

    int select = -1;
    bool isRepeat = true;
    Result result;

    while (isRepeat) {
      std::cin >> select;
      isRepeat = false;
      switch (select) {
      case 1:
        accuracyTest("source/easypr/resources/image/general_test", result);
        break;
      case 2:
        accuracyTest("source/easypr/resources/image/native_test", result);
        break;
      case 3:
        gridSearchTest("source/easypr/resources/image/general_test");
        break;
      case 4:
        isExit = true;
        break;
      default:
        std::cout << kv->get("input_error") << ":";
        isRepeat = true;
        break;
      }
    }
  }
  return 0;
}

int trainChineseMain() {
  std::shared_ptr<easypr::Kv> kv(new easypr::Kv);
  kv->load("source/easypr/resources/text/chinese_mapping");

  bool isExit = false;
  while (!isExit) {
    easypr::Utils::print_file_lines("source/easypr/resources/text/train_menu");
    std::cout << kv->get("make_a_choice") << ":";

    int select = -1;
    bool isRepeat = true;
    while (isRepeat) {
      std::cin >> select;
      isRepeat = false;
      switch (select) {
      case 1:
          {
            easypr::AnnChTrain ann("tmp/annCh", "tmp/annCh.xml");
            ann.setNumberForCount(100);
            ann.train();
          }
        break;
      case 2:
          {
            easypr::AnnChTrain ann("tmp/annCh", "tmp/annCh.xml");
            ann.setNumberForCount(350);
            ann.train();
          }
        break;
      case 3:
          {
            easypr::AnnChTrain ann("tmp/annCh", "tmp/annCh.xml");
            ann.setNumberForCount(700);
            ann.train();
          }
        break;
      case 4:
          {
            easypr::AnnChTrain ann("tmp/annCh", "tmp/annCh.xml");
            ann.setNumberForCount(1000);
            ann.train();
          }
        break;
      case 5:
          {
            easypr::AnnChTrain ann("tmp/annCh", "tmp/annCh.xml");
            ann.setNumberForCount(1500);
            ann.train();
          }
        break;
      case 6:
        isExit = true;
        break;
      default:
        std::cout << kv->get("input_error") << ":";
        isRepeat = true;
        break;
      }
    }
  }
  return 0;
}

int testMain() {
  std::shared_ptr<easypr::Kv> kv(new easypr::Kv);
  kv->load("source/easypr/resources/text/chinese_mapping");

  bool isExit = false;
  while (!isExit) {
    Utils::print_file_lines("source/easypr/resources/text/test_menu");
    std::cout << kv->get("make_a_choice") << ":";

    int select = -1;
    bool isRepeat = true;
    while (isRepeat) {
      std::cin >> select;
      isRepeat = false;
      switch (select) {
        case 1:
          test_plate_locate();
          break;
        case 2:
          test_plate_judge();
          break;
        case 3:
          test_plate_detect();
          break;
        case 4:
          test_chars_segment();
          break;
        case 5:
          test_chars_identify();
          break;
        case 6:
          test_chars_recognise();
          break;
        case 7:
          test_plate_recognize();
          break;
        case 8:
          test_plate_locate();
          test_plate_judge();
          test_plate_detect();

          test_chars_segment();
          test_chars_identify();
          test_chars_recognise();

          test_plate_recognize();
          break;
        case 9:
          isExit = true;
          break;
        default:
          std::cout << kv->get("input_error") << ":";
          isRepeat = true;
          break;
      }
    }
  }

  return 0;
}

}  // namespace demo

}  // namespace easypr

void command_line_handler(int argc, const char* argv[]) {
  program_options::Generator options;

  options.add_subroutine("svm", "svm operations").make_usage("Usage:");
  {
    /* ------------------------------------------
     | SVM Training operations
     | ------------------------------------------
     |
     | $ demo svm --plates=path/to/plates/ [--test] --svm=save/to/svm.xml
     |
     | ------------------------------------------
     */
    options("h,help", "show help information");
    options(",plates", "",
            "a folder contains both forward data and inverse data in the "
                "separated subfolders");
    options(",svm", easypr::kDefaultSvmPath, "the svm model file");
    options("t,test", "run tests in --plates");
  }

  options.add_subroutine("ann", "ann operation").make_usage("Usages:");
  {
    /* ------------------------------------------
    | ANN_MLP Training operations
    | ------------------------------------------
    |
    | $ demo ann --zh-chars=zhchars/ --en-chars=enchars/ --ann=save/to/ann.xml
    |
    | ------------------------------------------
    */
    options("h,help", "show help information");
    options(",chars", "",
            "the folder contains character sub-folders, with each folder"
                "named by label defined in include/easypr/config.h");
    options(",ann", easypr::kDefaultAnnPath,
            "the ann model file you want to save");
    options("t,test", "run test in --chars");
  }

  options.add_subroutine("locate", "locate plates in an image")
      .make_usage("Usage:");
  {
    /* ------------------------------------------
    | Plate locating operations
    | ------------------------------------------
    |
    | $ demo locate -f file
    |
    | ------------------------------------------
    */
    options("h,help", "show help information");
    options("f,file", "",
            "the target picture which contains one or more plates");
  }

  options.add_subroutine(
          "judge", "determine whether an image block is the license plate")
      .make_usage("Usage:");
  {
    /* ------------------------------------------
    | Plate judge operations
    | ------------------------------------------
    |
    | $ demo judge -f file --svm model/svm.xml
    |
    | ------------------------------------------
    */
    options("h,help", "show help information");
    options("f,file", "the target image block");
    options(",svm", easypr::kDefaultSvmPath, "the svm model file");
  }

  options.add_subroutine("recognize", "plate recognition").make_usage("Usage:");
  {
    /* ------------------------------------------
    | Plate recognize operations
    | ------------------------------------------
    |
    | $ demo recognize -p file --svm model/svm.xml
    |                          --ann model/ann.xml
    | $ demo recognize -pb dir/ --svm model/svm.xml
    |                           --ann model/ann.xml
    |
    | ------------------------------------------
    */
    options("h,help", "show help information");
    options("p,path", "", "where is the target picture or target folder");
    options("b,batch", "do batch recognition, if set, --path means a folder");
    options("c,color", "returns the plate color, blue or yellow");
    options(",svm", easypr::kDefaultSvmPath, "the svm model file");
    options(",ann", easypr::kDefaultAnnPath, "the ann model file");
  }

  auto parser = options.make_parser();

  try {
    parser->parse(argc, argv);
  } catch (const std::exception &err) {
    std::cout << err.what() << std::endl;
    return;
  }

  auto subname = parser->get_subroutine_name();

  program_options::select(subname)
      .found("svm",
             [&]() {
               if (parser->has("help") || argc <= 2) {
                 std::cout << options("svm");
                 return;
               }

               easypr::SvmTrain svm(parser->get("plates")->c_str(),
                                    parser->get("svm")->c_str());

               if (parser->has("test")) {
                 svm.test();
               } else {
                 svm.train();
               }
             })
      .found("ann",
             [&]() {
               if (parser->has("help") || argc <= 2) {
                 std::cout << options("ann");
                 return;
               }

               assert(parser->has("chars"));
               assert(parser->has("ann"));

               easypr::AnnTrain ann(parser->get("chars")->c_str(),
                                    parser->get("ann")->c_str());

               if (parser->has("test")) {
                 ann.test();
               } else {
                 ann.train();
               }
             })
      .found("locate",
             [&]() {
               if (parser->has("help") || argc <= 2) {
                 std::cout << options("locate");
                 return;
               }

               if (parser->has("file")) {
                 easypr::api::plate_locate(parser->get("file")->val().c_str());
                 std::cout << "finished, results can be found in tmp/"
                     << std::endl;
               }
             })
      .found("judge",
             [&]() {
               if (parser->has("help") || argc <= 2) {
                 std::cout << options("judge");
                 std::cout << "Note that the input image's size should "
                     << "be the same as the one you gived to svm train."
                     << std::endl;
                 return;
               }

               if (parser->has("file")) {
                 assert(parser->has("file"));
                 assert(parser->has("svm"));

                 auto image = parser->get("file")->val();
                 auto svm = parser->get("svm")->val();

                 const char* true_or_false[2] = {"false", "true"};

                 std::cout << true_or_false[easypr::api::plate_judge(
                     image.c_str(), svm.c_str())]
                     << std::endl;
               }
             })
      .found("recognize",
             [&]() {
               if (parser->has("help") || argc <= 2) {
                 std::cout << options("recognize");
                 return;
               }

               if (parser->has("path")) {
                 if (parser->has("batch")) {
                   // batch testing
                   auto folder = parser->get("path")->val();
                   easypr::demo::Result result;
                   easypr::demo::accuracyTest(folder.c_str(), result);
                 } else {
                   // single testing
                   auto image = parser->get("path")->val();

                   if (parser->has("color")) {
                     // return plate color
                     const char* colors[2] = {"blue", "yellow"};
                     std::cout
                         << colors[easypr::api::get_plate_color(image.c_str())]
                         << std::endl;
                   } else {
                     // return strings
                     auto svm = parser->get("svm")->val();
                     auto ann = parser->get("ann")->val();

                     auto results = easypr::api::plate_recognize(
                         image.c_str(), svm.c_str(), ann.c_str());
                     for (auto s : results) {
                       std::cout << s << std::endl;
                     }
                   }
                 }
               } else {
                 std::cout << "option 'file' cannot be empty." << std::endl;
               }
             })
      .others([&]() {
        // no case matched, print all commands.
        std::cout << "There are several sub commands listed below, "
            << "choose one by typing:\n\n"
            << "    " << easypr::utils::getFileName(argv[0])
            << " command [options]\n\n"
            << "The commands are:\n" << std::endl;
        auto subs = options.get_subroutine_list();
        for (auto sub : subs) {
          fprintf(stdout, "%s    %s\n", sub.first.c_str(), sub.second.c_str());
        }
        std::cout << std::endl;
      });
}

int main(int argc, const char* argv[]) {
  std::shared_ptr<easypr::Kv> kv(new easypr::Kv);
  kv->load("source/easypr/resources/text/chinese_mapping");

  if (argc > 1) {
    // handle command line execution.
    command_line_handler(argc, argv);
    return 0;
  }

  bool isExit = false;
  while (!isExit) {
    easypr::Utils::print_file_lines("source/easypr/resources/text/main_menu");
    std::cout << kv->get("make_a_choice") << ":";

    int select = -1;
    bool isRepeat = true;
    while (isRepeat) {
      std::cin >> select;
      isRepeat = false;
      switch (select) {
        case 1:
          easypr::demo::testMain();
          break;
        case 2:
          easypr::demo::accuracyTestMain();
          break;
        case 3:
          std::cout << "Run \"demo svm\" for more usage." << std::endl;
          {
            easypr::SvmTrain svm("tmp/svm", "tmp/svm.xml");
            svm.train();
          }
          break;
        case 4:
          std::cout << "Run \"demo ann\" for more usage." << std::endl;
          {
            easypr::AnnTrain ann("tmp/ann", "tmp/ann.xml");
            ann.train();
          }
          break;
        case 5:
          easypr::demo::trainChineseMain();
          break;
        case 6: {
          //TODO: genenrate gray characters
          easypr::demo::accuracyCharRecognizeTest("source/easypr/resources/image/tmp/plates_200k");
          break;
        }
        case 7: {
          easypr::Utils::print_file_lines("source/easypr/resources/text/thanks");
          break;
        }
        case 8:
          isExit = true;
          break;
        default:
          std::cout << kv->get("input_error") << ":";
          isRepeat = true;
          break;
      }
    }
  }
  return 0;
}